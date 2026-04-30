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

// Fast word-sized loads (assumes Little Endian)
static inline uint16_t ld16(const char* p) { uint16_t v; std::memcpy(&v, p, 2); return v; }
static inline uint32_t ld32(const char* p) { uint32_t v; std::memcpy(&v, p, 4); return v; }
static inline uint64_t ld64(const char* p) { uint64_t v; std::memcpy(&v, p, 8); return v; }

static constexpr uint8_t classOf(unsigned char c) {
    if (c >= 'a' && c <= 'z') return CC_ALPHA;
    if (c >= 'A' && c <= 'Z') return CC_ALPHA;
    if (c == '_')             return CC_ALPHA;
    if (c >= '0' && c <= '9') return CC_DIGIT;
    switch (c) {
        case '.':  return CC_DOT;     case '"':  return CC_DQUOTE;
        case '/':  return CC_SLASH;   case '*':  return CC_STAR;   case '+':  return CC_PLUS;
        case '-':  return CC_MINUS;   case '%':  return CC_PERCENT;
        case '=':  return CC_EQ;      case '!':  return CC_BANG;
        case '<':  return CC_LT;      case '>':  return CC_GT;
        case '&':  return CC_AMP;     case '|':  return CC_PIPE;
        case ':':  return CC_COLON;   case '(':  return CC_LPAREN;
        case ')':  return CC_RPAREN;  case '{':  return CC_LBRACE;
        case '}':  return CC_RBRACE;  case '[':  return CC_LBRACK;
        case ']':  return CC_RBRACK;  case ';':  return CC_SEMI;
        case ',':  return CC_COMMA;   case '\n': return CC_NL;
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

// Keyword resolution via word-sized loads to minimize memory operations
TokenType Lexer::resolveKeyword(llvm::StringRef w) {
    const char *s = w.data();
    switch (w.size()) {
    case 2: {
        uint16_t v = ld16(s);
        if (v == 0x6E66) return TOKEN_FUNCTION; // "fn"
        if (v == 0x6669) return TOKEN_IF;       // "if"
        break;
    }
    case 3: {
        uint32_t v = 0; std::memcpy(&v, s, 3);
        if (v == 0x726F66) return TOKEN_FOR;      // "for"
        if (v == 0x636564) return TOKEN_DECL;     // "dec"
        if (v == 0x666661) return TOKEN_ASSIGN;   // "aff"
        if (v == 0x666572) return TOKEN_REF;      // "ref"
        if (v == 0x677261) return TOKEN_ARG;      // "arg"
        if (v == 0x77656E) return TOKEN_NEW;      // "new"
        if (v == 0x727470) return TOKEN_TYPE_PTR; // "ptr"
        break;
    }
    case 4: {
        uint32_t v = ld32(s);
        if (v == 0x65736C65) return TOKEN_ELSE;      // "else"
        if (v == 0x65757274) return TOKEN_TRUE;      // "true"
        if (v == 0x64696F76) return TOKEN_TYPE_VOID; // "void"
        if (v == 0x6C6F6F62) return TOKEN_TYPE_BOOL; // "bool"
        if (v == 0x72616863) return TOKEN_TYPE_CHAR; // "char"
        if (v == 0x6C6C6163) return TOKEN_CALL;      // "call"
        if (v == 0x73736170) return TOKEN_PASS;      // "pass"
        break;
    }
    case 5: {
        uint64_t v = 0; std::memcpy(&v, s, 5);
        if (v == 0x656C696877) return TOKEN_WHILE;     // "while"
        if (v == 0x65736C6166) return TOKEN_FALSE;     // "false"
        if (v == 0x65706F6373) return TOKEN_SCOPE;     // "scope"
        if (v == 0x6665726E75) return TOKEN_UNREF;     // "unref"
        if (v == 0x36746E69) { // "int6"
             if (s[4] == '4') return TOKEN_TYPE_INT64;
        }
        if (v == 0x32746E69) { // "int3"
             if (s[4] == '2') return TOKEN_TYPE_INT32;
        }
        if (v == 0x74616F6C66) return TOKEN_TYPE_FLOAT; // "float"
        if (v == 0x7373616C63) return TOKEN_CLASS;      // "class"
        break;
    }
    case 6: {
        uint64_t v = 0; std::memcpy(&v, s, 6);
        if (v == 0x6E7275746572) return TOKEN_RETURN;      // "return"
        if (v == 0x676E69727473) return TOKEN_TYPE_STRING; // "string"
        if (v == 0x6574656C6564) return TOKEN_DELETE;      // "delete"
        if (v == 0x63696C627570) return TOKEN_PUBLIC;      // "public"
        break;
    }
    case 7:
        if (!memcmp(s, "include", 7)) return TOKEN_INCLUDE;
        if (!memcmp(s, "private", 7)) return TOKEN_PRIVATE;
        break;
    case 9:
        if (!memcmp(s, "protected", 9)) return TOKEN_PROTECTED;
        break;
    }
    return TOKEN_IDENTIFIER;
}

static inline bool isUnary(TokenType t) {
    switch (t) {
        case TOKEN_EQUAL: case TOKEN_PAREN_OPEN: case TOKEN_COMMA:
        case TOKEN_BRACKET_OPEN: case TOKEN_PLUS: case TOKEN_MINUS:
        case TOKEN_STAR: case TOKEN_SLASH: case TOKEN_MODULO:
        case TOKEN_EQUAL_EQUAL: case TOKEN_NOT_EQUAL:
        case TOKEN_LESS: case TOKEN_GREATER:
        case TOKEN_LESS_EQUAL: case TOKEN_GREATER_EQUAL:
        case TOKEN_AND: case TOKEN_OR: case TOKEN_EOF:
            return true;
        default:
            return false;
    }
}

auto Lexer::tokenize(const string& sourceCode) -> vector<Token> {
    vector<Token> tokens;
    tokens.reserve(sourceCode.size() >> 2);

    const char *cursor = sourceCode.data();
    const char *end    = cursor + sourceCode.size();
    const char *lineStart = cursor;
    int line = 1;
    TokenType lastTy = TOKEN_EOF;

    while (cursor < end) {
        unsigned char ch = *cursor;
        uint8_t cc = CC(ch);

        switch (cc) {
        case CC_WS:
            cursor++;
            break;
        case CC_NL:
            line++;
            cursor++;
            lineStart = cursor;
            break;
        case CC_ALPHA: {
            const char* mark = cursor;
            int col = static_cast<int>(cursor - lineStart) + 1;
            while (++cursor < end && (CC(*cursor) == CC_ALPHA || CC(*cursor) == CC_DIGIT));
            llvm::StringRef word(mark, (size_t)(cursor - mark));
            TokenType ty = resolveKeyword(word);
            if (ty == TOKEN_TRUE)       { tokens.push_back({TOKEN_LIT_BOOL, "1", line, col}); lastTy = TOKEN_LIT_BOOL; }
            else if (ty == TOKEN_FALSE) { tokens.push_back({TOKEN_LIT_BOOL, "0", line, col}); lastTy = TOKEN_LIT_BOOL; }
            else                        { tokens.push_back({ty, word, line, col}); lastTy = ty; }
            break;
        }
        case CC_DIGIT: {
            const char* mark = cursor;
            int col = static_cast<int>(cursor - lineStart) + 1;
            while (++cursor < end && CC(*cursor) == CC_DIGIT);
            bool isFp = false;
            if (cursor < end && *cursor == '.') {
                if (cursor + 1 < end && CC(*(cursor + 1)) == CC_DIGIT) {
                    isFp = true;
                    cursor++;
                    while (cursor < end && CC(*cursor) == CC_DIGIT) cursor++;
                }
            }
            lastTy = isFp ? TOKEN_LIT_FLOAT : TOKEN_LIT_INT;
            tokens.push_back({lastTy, llvm::StringRef(mark, (size_t)(cursor - mark)), line, col});
            break;
        }
        case CC_DQUOTE: {
            int col = static_cast<int>(cursor - lineStart) + 1;
            tokens.push_back({TOKEN_QUOTE, "\"", line, col});
            cursor++;
            const char* mark = cursor;
            int markCol = static_cast<int>(cursor - lineStart) + 1;
            while (cursor < end && *cursor != '"') {
                if (*cursor == '\\' && cursor + 1 < end) cursor++;
                if (*cursor == '\n') { line++; lineStart = cursor + 1; }
                cursor++;
            }
            if (cursor > mark) {
                tokens.push_back({TOKEN_IDENTIFIER, llvm::StringRef(mark, (size_t)(cursor - mark)), line, markCol});
            }
            if (cursor < end && *cursor == '"') {
                tokens.push_back({TOKEN_QUOTE, "\"", line, static_cast<int>(cursor - lineStart) + 1});
                cursor++;
            }
            lastTy = TOKEN_QUOTE;
            break;
        }
        case CC_SLASH: {
            int col = static_cast<int>(cursor - lineStart) + 1;
            if (cursor + 1 < end) {
                if (cursor[1] == '/') {
                    cursor += 2;
                    while (cursor < end && *cursor != '\n') cursor++;
                    continue;
                }
                if (cursor[1] == '*') {
                    cursor += 2;
                    while (cursor < end) {
                        if (*cursor == '*' && cursor + 1 < end && cursor[1] == '/') {
                            cursor += 2;
                            break;
                        }
                        if (*cursor == '\n') { line++; lineStart = cursor + 1; }
                        cursor++;
                    }
                    continue;
                }
            }
            tokens.push_back({TOKEN_SLASH, "/", line, col});
            lastTy = TOKEN_SLASH;
            cursor++;
            break;
        }
        case CC_PLUS:
        case CC_MINUS: {
            int col = static_cast<int>(cursor - lineStart) + 1;
            if (isUnary(lastTy) && cursor + 1 < end && CC(cursor[1]) == CC_DIGIT) {
                const char* mark = cursor;
                cursor++;
                while (cursor < end && CC(*cursor) == CC_DIGIT) cursor++;
                bool isFp = false;
                if (cursor < end && *cursor == '.') {
                    if (cursor + 1 < end && CC(cursor[1]) == CC_DIGIT) {
                        isFp = true;
                        cursor++;
                        while (cursor < end && CC(*cursor) == CC_DIGIT) cursor++;
                    }
                }
                lastTy = isFp ? TOKEN_LIT_FLOAT : TOKEN_LIT_INT;
                tokens.push_back({lastTy, llvm::StringRef(mark, (size_t)(cursor - mark)), line, col});
            } else {
                lastTy = (cc == CC_PLUS) ? TOKEN_PLUS : TOKEN_MINUS;
                tokens.push_back({lastTy, (cc == CC_PLUS) ? "+" : "-", line, col});
                cursor++;
            }
            break;
        }
        case CC_EQ: {
            int col = static_cast<int>(cursor - lineStart) + 1;
            if (cursor + 1 < end && cursor[1] == '=') {
                tokens.push_back({TOKEN_EQUAL_EQUAL, "==", line, col});
                lastTy = TOKEN_EQUAL_EQUAL;
                cursor += 2;
            } else {
                tokens.push_back({TOKEN_EQUAL, "=", line, col});
                lastTy = TOKEN_EQUAL;
                cursor++;
            }
            break;
        }
        case CC_BANG: {
            int col = static_cast<int>(cursor - lineStart) + 1;
            if (cursor + 1 < end && cursor[1] == '=') {
                tokens.push_back({TOKEN_NOT_EQUAL, "!=", line, col});
                lastTy = TOKEN_NOT_EQUAL;
                cursor += 2;
            } else {
                tokens.push_back({TOKEN_NOT, "!", line, col});
                lastTy = TOKEN_NOT;
                cursor++;
            }
            break;
        }
        case CC_LT: {
            int col = static_cast<int>(cursor - lineStart) + 1;
            if (cursor + 1 < end && cursor[1] == '=') {
                tokens.push_back({TOKEN_LESS_EQUAL, "<=", line, col});
                lastTy = TOKEN_LESS_EQUAL;
                cursor += 2;
            } else {
                tokens.push_back({TOKEN_LESS, "<", line, col});
                lastTy = TOKEN_LESS;
                cursor++;
            }
            break;
        }
        case CC_GT: {
            int col = static_cast<int>(cursor - lineStart) + 1;
            if (cursor + 1 < end && cursor[1] == '=') {
                tokens.push_back({TOKEN_GREATER_EQUAL, ">=", line, col});
                lastTy = TOKEN_GREATER_EQUAL;
                cursor += 2;
            } else {
                tokens.push_back({TOKEN_GREATER, ">", line, col});
                lastTy = TOKEN_GREATER;
                cursor++;
            }
            break;
        }
        case CC_AMP: {
            int col = static_cast<int>(cursor - lineStart) + 1;
            if (cursor + 1 < end && cursor[1] == '&') {
                tokens.push_back({TOKEN_AND, "&&", line, col});
                lastTy = TOKEN_AND;
                cursor += 2;
            } else cursor++;
            break;
        }
        case CC_PIPE: {
            int col = static_cast<int>(cursor - lineStart) + 1;
            if (cursor + 1 < end && cursor[1] == '|') {
                tokens.push_back({TOKEN_OR, "||", line, col});
                lastTy = TOKEN_OR;
                cursor += 2;
            } else cursor++;
            break;
        }
        case CC_COLON: {
            int col = static_cast<int>(cursor - lineStart) + 1;
            if (cursor + 1 < end && cursor[1] == ':') {
                tokens.push_back({TOKEN_COLON, "::", line, col});
                lastTy = TOKEN_COLON;
                cursor += 2;
            } else {
                tokens.push_back({TOKEN_COLON, ":", line, col});
                lastTy = TOKEN_COLON;
                cursor++;
            }
            break;
        }
        case CC_DOT: tokens.push_back({TOKEN_DOT, ".", line, static_cast<int>(cursor++ - lineStart) + 1}); lastTy = TOKEN_DOT; break;
        case CC_STAR: tokens.push_back({TOKEN_STAR, "*", line, static_cast<int>(cursor++ - lineStart) + 1}); lastTy = TOKEN_STAR; break;
        case CC_PERCENT: tokens.push_back({TOKEN_MODULO, "%", line, static_cast<int>(cursor++ - lineStart) + 1}); lastTy = TOKEN_MODULO; break;
        case CC_LPAREN: tokens.push_back({TOKEN_PAREN_OPEN, "(", line, static_cast<int>(cursor++ - lineStart) + 1}); lastTy = TOKEN_PAREN_OPEN; break;
        case CC_RPAREN: tokens.push_back({TOKEN_PAREN_CLOSE, ")", line, static_cast<int>(cursor++ - lineStart) + 1}); lastTy = TOKEN_PAREN_CLOSE; break;
        case CC_LBRACE: tokens.push_back({TOKEN_BRACE_OPEN, "{", line, static_cast<int>(cursor++ - lineStart) + 1}); lastTy = TOKEN_BRACE_OPEN; break;
        case CC_RBRACE: tokens.push_back({TOKEN_BRACE_CLOSE, "}", line, static_cast<int>(cursor++ - lineStart) + 1}); lastTy = TOKEN_BRACE_CLOSE; break;
        case CC_LBRACK: tokens.push_back({TOKEN_BRACKET_OPEN, "[", line, static_cast<int>(cursor++ - lineStart) + 1}); lastTy = TOKEN_BRACKET_OPEN; break;
        case CC_RBRACK: tokens.push_back({TOKEN_BRACKET_CLOSE, "]", line, static_cast<int>(cursor++ - lineStart) + 1}); lastTy = TOKEN_BRACKET_CLOSE; break;
        case CC_SEMI: tokens.push_back({TOKEN_SEMICOLON, ";", line, static_cast<int>(cursor++ - lineStart) + 1}); lastTy = TOKEN_SEMICOLON; break;
        case CC_COMMA: tokens.push_back({TOKEN_COMMA, ",", line, static_cast<int>(cursor++ - lineStart) + 1}); lastTy = TOKEN_COMMA; break;
        default: cursor++; break;
        }
    }

    tokens.push_back({TOKEN_EOF, "", line, (int)(cursor - lineStart) + 1});
    return tokens;
}
