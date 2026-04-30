//===-- lexer.cpp -----------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#include "compiler/lexer/lexer.h"
#include "compiler/lexer/token_type.h"
#include <cstring>

// constexpr byte -> char-class mapping, built at compile time
static constexpr uint8_t classOf(unsigned char c) {
    if (c >= 'a' && c <= 'z') return CC_ALPHA;
    if (c >= 'A' && c <= 'Z') return CC_ALPHA;
    if (c == '_')             return CC_ALPHA;
    if (c >= '0' && c <= '9') return CC_DIGIT;
    switch (c) {
        case '.':  return CC_DOT;     case '"':  return CC_DQUOTE;
        case '/':  return CC_SLASH;   case '*':  return CC_STAR;
        case '+':  return CC_PLUS;    case '-':  return CC_MINUS;
        case '%':  return CC_PERCENT; case '=':  return CC_EQ;
        case '!':  return CC_BANG;    case '<':  return CC_LT;
        case '>':  return CC_GT;     case '&':  return CC_AMP;
        case '|':  return CC_PIPE;    case ':':  return CC_COLON;
        case '(':  return CC_LPAREN;  case ')':  return CC_RPAREN;
        case '{':  return CC_LBRACE;  case '}':  return CC_RBRACE;
        case '[':  return CC_LBRACK;  case ']':  return CC_RBRACK;
        case ';':  return CC_SEMI;    case ',':  return CC_COMMA;
        case '\n': return CC_NL;      case '\\': return CC_BSLASH;
        case ' ': case '\t': case '\r': return CC_WS;
        default:   return CC_OTHER;
    }
}

struct CClassLUT {
    uint8_t v[256];
    constexpr CClassLUT() : v{} {
        for (unsigned i = 0; i < 256; ++i)
            v[i] = classOf(static_cast<unsigned char>(i));
    }
};
static constexpr CClassLUT cclut{};
#define CC(c) cclut.v[static_cast<unsigned char>(c)]

// keyword resolution -- length-bucketed so the compiler can jump-table it
TokenType Lexer::resolveKeyword(llvm::StringRef w) {
    const char *s = w.data();
    switch (w.size()) {
    case 2:
        if (s[0]=='f' && s[1]=='n') return TOKEN_FUNCTION;
        if (s[0]=='i' && s[1]=='f') return TOKEN_IF;
        break;
    case 3:
        if (!memcmp(s,"for",3)) return TOKEN_FOR;
        if (!memcmp(s,"dec",3)) return TOKEN_DECL;
        if (!memcmp(s,"aff",3)) return TOKEN_ASSIGN;
        if (!memcmp(s,"ref",3)) return TOKEN_REF;
        if (!memcmp(s,"arg",3)) return TOKEN_ARG;
        if (!memcmp(s,"new",3)) return TOKEN_NEW;
        if (!memcmp(s,"ptr",3)) return TOKEN_TYPE_PTR;
        break;
    case 4:
        if (!memcmp(s,"else",4)) return TOKEN_ELSE;
        if (!memcmp(s,"true",4)) return TOKEN_TRUE;
        if (!memcmp(s,"void",4)) return TOKEN_TYPE_VOID;
        if (!memcmp(s,"bool",4)) return TOKEN_TYPE_BOOL;
        if (!memcmp(s,"char",4)) return TOKEN_TYPE_CHAR;
        if (!memcmp(s,"call",4)) return TOKEN_CALL;
        if (!memcmp(s,"pass",4)) return TOKEN_PASS;
        break;
    case 5:
        if (!memcmp(s,"while",5)) return TOKEN_WHILE;
        if (!memcmp(s,"false",5)) return TOKEN_FALSE;
        if (!memcmp(s,"scope",5)) return TOKEN_SCOPE;
        if (!memcmp(s,"unref",5)) return TOKEN_UNREF;
        if (!memcmp(s,"int64",5)) return TOKEN_TYPE_INT64;
        if (!memcmp(s,"int32",5)) return TOKEN_TYPE_INT32;
        if (!memcmp(s,"float",5)) return TOKEN_TYPE_FLOAT;
        if (!memcmp(s,"class",5)) return TOKEN_CLASS;
        break;
    case 6:
        if (!memcmp(s,"return",6)) return TOKEN_RETURN;
        if (!memcmp(s,"string",6)) return TOKEN_TYPE_STRING;
        if (!memcmp(s,"delete",6)) return TOKEN_DELETE;
        if (!memcmp(s,"public",6)) return TOKEN_PUBLIC;
        break;
    case 7:
        if (!memcmp(s,"include",7)) return TOKEN_INCLUDE;
        if (!memcmp(s,"private",7)) return TOKEN_PRIVATE;
        break;
    case 9:
        if (!memcmp(s,"protected",9)) return TOKEN_PROTECTED;
        break;
    }
    return TOKEN_IDENTIFIER;
}

bool Lexer::isUnaryContext(const vector<Token>& tokens) {
    if (tokens.empty()) return true;
    switch (tokens.back().type) {
        case TOKEN_EQUAL: case TOKEN_PAREN_OPEN: case TOKEN_COMMA:
        case TOKEN_BRACKET_OPEN: case TOKEN_PLUS: case TOKEN_MINUS:
        case TOKEN_STAR: case TOKEN_SLASH: case TOKEN_MODULO:
        case TOKEN_EQUAL_EQUAL: case TOKEN_NOT_EQUAL:
        case TOKEN_LESS: case TOKEN_GREATER:
        case TOKEN_LESS_EQUAL: case TOKEN_GREATER_EQUAL:
        case TOKEN_AND: case TOKEN_OR:
            return true;
        default:
            return false;
    }
}

auto Lexer::tokenize(const string& sourceCode) -> vector<Token> {
    vector<Token> tokens;
    tokens.reserve(sourceCode.size() / 4);

    const char *src = sourceCode.data();
    const size_t len = sourceCode.size();
    size_t pos = 0;
    int line = 1, col = 1;

    auto emitIdent = [&](size_t start, int sc) {
        llvm::StringRef word(src + start, pos - start);
        TokenType ty = resolveKeyword(word);
        if (ty == TOKEN_TRUE)       tokens.push_back({TOKEN_LIT_BOOL, "1", line, sc});
        else if (ty == TOKEN_FALSE) tokens.push_back({TOKEN_LIT_BOOL, "0", line, sc});
        else                        tokens.push_back({ty, word, line, sc});
    };

    auto emitNum = [&](size_t start, int sc, bool fp) {
        llvm::StringRef val(src + start, pos - start);
        tokens.push_back({fp ? TOKEN_LIT_FLOAT : TOKEN_LIT_INT, val, line, sc});
    };

    auto emit1 = [&](TokenType t, const char *v) {
        tokens.push_back({t, v, line, col});
    };

    DfaState state = S_START;
    size_t mark = 0;
    int markCol = 1;
    bool numFloat = false;

    while (pos < len) {
        unsigned char ch = static_cast<unsigned char>(src[pos]);
        uint8_t cc = CC(ch);

        switch (state) {

        case S_START:
            switch (cc) {
            case CC_ALPHA:
                mark = pos; markCol = col;
                state = S_IDENT;
                pos++; col++;
                break;
            case CC_DIGIT:
                mark = pos; markCol = col;
                numFloat = false;
                state = S_INT;
                pos++; col++;
                break;
            case CC_DOT:    emit1(TOKEN_DOT, "."); pos++; col++; break;
            case CC_DQUOTE:
                tokens.push_back({TOKEN_QUOTE, "\"", line, col});
                pos++; col++;
                mark = pos; markCol = col;
                state = S_STRING;
                break;
            case CC_SLASH:
                mark = pos; markCol = col;
                state = S_SLASH;
                pos++; col++;
                break;
            case CC_STAR:    emit1(TOKEN_STAR,   "*"); pos++; col++; break;
            case CC_PERCENT: emit1(TOKEN_MODULO, "%"); pos++; col++; break;
            case CC_PLUS:
                mark = pos; markCol = col; state = S_PLUS; pos++; col++; break;
            case CC_MINUS:
                mark = pos; markCol = col; state = S_MINUS; pos++; col++; break;
            case CC_EQ:
                mark = pos; markCol = col; state = S_EQ; pos++; col++; break;
            case CC_BANG:
                mark = pos; markCol = col; state = S_BANG; pos++; col++; break;
            case CC_LT:
                mark = pos; markCol = col; state = S_LT; pos++; col++; break;
            case CC_GT:
                mark = pos; markCol = col; state = S_GT; pos++; col++; break;
            case CC_AMP:
                mark = pos; markCol = col; state = S_AMP; pos++; col++; break;
            case CC_PIPE:
                mark = pos; markCol = col; state = S_PIPE; pos++; col++; break;
            case CC_COLON:
                mark = pos; markCol = col; state = S_COLON; pos++; col++; break;
            case CC_LPAREN: emit1(TOKEN_PAREN_OPEN,    "("); pos++; col++; break;
            case CC_RPAREN: emit1(TOKEN_PAREN_CLOSE,   ")"); pos++; col++; break;
            case CC_LBRACE: emit1(TOKEN_BRACE_OPEN,    "{"); pos++; col++; break;
            case CC_RBRACE: emit1(TOKEN_BRACE_CLOSE,   "}"); pos++; col++; break;
            case CC_LBRACK: emit1(TOKEN_BRACKET_OPEN,  "["); pos++; col++; break;
            case CC_RBRACK: emit1(TOKEN_BRACKET_CLOSE, "]"); pos++; col++; break;
            case CC_SEMI:   emit1(TOKEN_SEMICOLON,     ";"); pos++; col++; break;
            case CC_COMMA:  emit1(TOKEN_COMMA,         ","); pos++; col++; break;
            case CC_NL:     line++; col = 1; pos++; break;
            case CC_WS:     pos++; col++; break;
            default:        pos++; col++; break;
            }
            break;

        case S_IDENT:
            if (cc == CC_ALPHA || cc == CC_DIGIT) { pos++; col++; }
            else { emitIdent(mark, markCol); state = S_START; }
            break;

        case S_INT:
            if (cc == CC_DIGIT) { pos++; col++; }
            else if (cc == CC_DOT) { state = S_DOT_AFTER_INT; pos++; col++; }
            else { emitNum(mark, markCol, false); state = S_START; }
            break;

        case S_DOT_AFTER_INT:
            if (cc == CC_DIGIT) { state = S_FLOAT; pos++; col++; }
            else {
                // "3." at a non-digit -- emit int without the dot, then the dot
                size_t dotPos = pos - 1;
                size_t save = pos; int saveCol = col;
                pos = dotPos;
                emitNum(mark, markCol, false);
                pos = save; col = saveCol;
                tokens.push_back({TOKEN_DOT, ".", line, col - 1});
                state = S_START;
            }
            break;

        case S_FLOAT:
            if (cc == CC_DIGIT) { pos++; col++; }
            else { emitNum(mark, markCol, true); state = S_START; }
            break;

        case S_STRING:
            if (cc == CC_DQUOTE) {
                if (pos > mark) {
                    llvm::StringRef val(src + mark, pos - mark);
                    tokens.push_back({TOKEN_IDENTIFIER, val, line, markCol});
                }
                tokens.push_back({TOKEN_QUOTE, "\"", line, col});
                pos++; col++;
                state = S_START;
            } else if (cc == CC_BSLASH && pos + 1 < len) {
                state = S_STRING_ESC;
                pos++; col++;
            } else {
                if (ch == '\n') { line++; col = 0; }
                pos++; col++;
            }
            break;

        case S_STRING_ESC:
            pos++; col++;
            state = S_STRING;
            break;

        case S_SLASH:
            if (cc == CC_SLASH)     { state = S_LINE_CMT; pos++; col++; }
            else if (cc == CC_STAR) { state = S_BLOCK_CMT; pos++; col++; }
            else { tokens.push_back({TOKEN_SLASH, "/", line, markCol}); state = S_START; }
            break;

        case S_LINE_CMT:
            if (cc == CC_NL) { line++; col = 1; pos++; state = S_START; }
            else { pos++; col++; }
            break;

        case S_BLOCK_CMT:
            if (cc == CC_STAR)    state = S_BLOCK_STAR;
            else if (cc == CC_NL) { line++; col = 0; }
            pos++; col++;
            break;

        case S_BLOCK_STAR:
            if (cc == CC_SLASH) { pos++; col++; state = S_START; }
            else if (cc == CC_STAR) { pos++; col++; }
            else {
                if (cc == CC_NL) { line++; col = 0; }
                pos++; col++;
                state = S_BLOCK_CMT;
            }
            break;

        case S_EQ:
            if (cc == CC_EQ) { tokens.push_back({TOKEN_EQUAL_EQUAL, "==", line, markCol}); pos++; col++; }
            else             { tokens.push_back({TOKEN_EQUAL, "=", line, markCol}); }
            state = S_START;
            break;

        case S_BANG:
            if (cc == CC_EQ) { tokens.push_back({TOKEN_NOT_EQUAL, "!=", line, markCol}); pos++; col++; }
            else             { tokens.push_back({TOKEN_NOT, "!", line, markCol}); }
            state = S_START;
            break;

        case S_LT:
            if (cc == CC_EQ) { tokens.push_back({TOKEN_LESS_EQUAL, "<=", line, markCol}); pos++; col++; }
            else             { tokens.push_back({TOKEN_LESS, "<", line, markCol}); }
            state = S_START;
            break;

        case S_GT:
            if (cc == CC_EQ) { tokens.push_back({TOKEN_GREATER_EQUAL, ">=", line, markCol}); pos++; col++; }
            else             { tokens.push_back({TOKEN_GREATER, ">", line, markCol}); }
            state = S_START;
            break;

        case S_AMP:
            if (cc == CC_AMP) { tokens.push_back({TOKEN_AND, "&&", line, markCol}); pos++; col++; }
            state = S_START;
            break;

        case S_PIPE:
            if (cc == CC_PIPE) { tokens.push_back({TOKEN_OR, "||", line, markCol}); pos++; col++; }
            state = S_START;
            break;

        case S_COLON:
            if (cc == CC_COLON) { tokens.push_back({TOKEN_COLON, "::", line, markCol}); pos++; col++; }
            else                { tokens.push_back({TOKEN_COLON, ":", line, markCol}); }
            state = S_START;
            break;

        case S_PLUS:
            if (cc == CC_DIGIT && isUnaryContext(tokens)) {
                numFloat = false; state = S_SIGN_INT; pos++; col++;
            } else {
                tokens.push_back({TOKEN_PLUS, "+", line, markCol}); state = S_START;
            }
            break;

        case S_MINUS:
            if (cc == CC_DIGIT && isUnaryContext(tokens)) {
                numFloat = false; state = S_SIGN_INT; pos++; col++;
            } else {
                tokens.push_back({TOKEN_MINUS, "-", line, markCol}); state = S_START;
            }
            break;

        case S_SIGN_INT:
            if (cc == CC_DIGIT) { pos++; col++; }
            else if (cc == CC_DOT) { state = S_SIGN_DOT; pos++; col++; }
            else { emitNum(mark, markCol, false); state = S_START; }
            break;

        case S_SIGN_DOT:
            if (cc == CC_DIGIT) { state = S_SIGN_FLOAT; pos++; col++; }
            else {
                size_t dotPos = pos - 1;
                size_t save = pos; int saveCol = col;
                pos = dotPos;
                emitNum(mark, markCol, false);
                pos = save; col = saveCol;
                tokens.push_back({TOKEN_DOT, ".", line, col - 1});
                state = S_START;
            }
            break;

        case S_SIGN_FLOAT:
            if (cc == CC_DIGIT) { pos++; col++; }
            else { emitNum(mark, markCol, true); state = S_START; }
            break;

        default:
            pos++; col++;
            state = S_START;
            break;
        }
    }

    // flush pending state at EOF
    switch (state) {
        case S_IDENT:      emitIdent(mark, markCol); break;
        case S_INT:
        case S_SIGN_INT:   emitNum(mark, markCol, false); break;
        case S_FLOAT:
        case S_SIGN_FLOAT: emitNum(mark, markCol, true); break;
        case S_DOT_AFTER_INT:
        case S_SIGN_DOT:   emitNum(mark, markCol, true); break;
        case S_STRING:
            if (pos > mark) {
                llvm::StringRef val(src + mark, pos - mark);
                tokens.push_back({TOKEN_IDENTIFIER, val, line, markCol});
            }
            break;
        case S_SLASH: tokens.push_back({TOKEN_SLASH, "/", line, markCol}); break;
        case S_EQ:    tokens.push_back({TOKEN_EQUAL, "=", line, markCol}); break;
        case S_BANG:  tokens.push_back({TOKEN_NOT,   "!", line, markCol}); break;
        case S_LT:    tokens.push_back({TOKEN_LESS,  "<", line, markCol}); break;
        case S_GT:    tokens.push_back({TOKEN_GREATER, ">", line, markCol}); break;
        case S_COLON: tokens.push_back({TOKEN_COLON, ":", line, markCol}); break;
        case S_PLUS:  tokens.push_back({TOKEN_PLUS,  "+", line, markCol}); break;
        case S_MINUS: tokens.push_back({TOKEN_MINUS, "-", line, markCol}); break;
        default: break;
    }

    tokens.push_back({TOKEN_EOF, "", line, col});
    return tokens;
}
