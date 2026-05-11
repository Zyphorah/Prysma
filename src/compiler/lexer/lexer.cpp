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

// Branch prediction hints
#define UNLIKELY(x) __builtin_expect(!!(x), 0)

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

// Keyword resolution via masked word-sized loads
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
    case 7: {
        uint64_t v = ld64(s);
        if ((v & 0x00FFFFFFFFFFFFFFULL) == 0x6564756C636E69ULL) return TOKEN_INCLUDE; // "include"
        if ((v & 0x00FFFFFFFFFFFFFFULL) == 0x65746176697270ULL) return TOKEN_PRIVATE; // "private"
        break;
    }
    case 9: {
        if (ld64(s) == 0x65746365746F7270ULL && s[8] == 'd') return TOKEN_PROTECTED; // "protected"
        break;
    }
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
    // Pre-allocate maximum possible tokens (1 char = 1 token worst case) + EOF
    tokens.resize(sourceCode.size() + 1);
    
    Token* __restrict out = tokens.data();
    const char* __restrict cursor = sourceCode.data();
    const uint8_t* __restrict lut = cclut.v;
    TokenType lastTy = TOKEN_EOF;

    while (true) {
        uint8_t c = static_cast<uint8_t>(*cursor);
        uint8_t cc = lut[c];

        switch (cc) {
        case CC_WS:
        case CC_NL:
            cursor++;
            break;
        case CC_ALPHA: {
            const char* mark = cursor;
            while (true) {
                uint8_t nextClass = lut[static_cast<uint8_t>(*++cursor)];
                if (nextClass != CC_ALPHA && nextClass != CC_DIGIT) break;
            }
            llvm::StringRef word(mark, cursor - mark);
            TokenType ty = resolveKeyword(word);
            if (ty == TOKEN_TRUE)       { *out++ = {TOKEN_LIT_BOOL, "1", 0, 0}; lastTy = TOKEN_LIT_BOOL; }
            else if (ty == TOKEN_FALSE) { *out++ = {TOKEN_LIT_BOOL, "0", 0, 0}; lastTy = TOKEN_LIT_BOOL; }
            else                        { *out++ = {ty, word, 0, 0}; lastTy = ty; }
            break;
        }
        case CC_DIGIT: {
            const char* mark = cursor;
            while (lut[static_cast<uint8_t>(*++cursor)] == CC_DIGIT);
            bool isFp = false;
            if (UNLIKELY(*cursor == '.')) {
                if (lut[static_cast<uint8_t>(cursor[1])] == CC_DIGIT) {
                    isFp = true;
                    cursor++;
                    while (lut[static_cast<uint8_t>(*++cursor)] == CC_DIGIT);
                }
            }
            lastTy = isFp ? TOKEN_LIT_FLOAT : TOKEN_LIT_INT;
            *out++ = {lastTy, llvm::StringRef(mark, cursor - mark), 0, 0};
            break;
        }
        case CC_DQUOTE: {
            *out++ = {TOKEN_QUOTE, "\"", 0, 0};
            cursor++;
            const char* mark = cursor;
            // Extremely fast SIMD-like string scanning using strcspn
            while (true) {
                size_t skip = std::strcspn(cursor, "\"\\\n");
                cursor += skip;
                if (UNLIKELY(*cursor == '\\' && cursor[1] != '\0')) {
                    cursor += 2;
                } else {
                    break;
                }
            }
            if (cursor > mark) {
                *out++ = {TOKEN_IDENTIFIER, llvm::StringRef(mark, cursor - mark), 0, 0};
            }
            if (*cursor == '"') {
                *out++ = {TOKEN_QUOTE, "\"", 0, 0};
                cursor++;
            }
            lastTy = TOKEN_QUOTE;
            break;
        }
        case CC_SLASH: {
            if (cursor[1] == '/') {
                cursor += 2;
                while (*cursor != '\n' && *cursor != '\0') cursor++;
                continue;
            }
            if (cursor[1] == '*') {
                cursor += 2;
                while (*cursor != '\0') {
                    if (*cursor == '*' && cursor[1] == '/') {
                        cursor += 2;
                        break;
                    }
                    cursor++;
                }
                continue;
            }
            *out++ = {TOKEN_SLASH, "/", 0, 0};
            lastTy = TOKEN_SLASH;
            cursor++;
            break;
        }
        case CC_PLUS:
        case CC_MINUS: {
            if (isUnary(lastTy) && lut[static_cast<uint8_t>(cursor[1])] == CC_DIGIT) {
                const char* mark = cursor;
                while (lut[static_cast<uint8_t>(*++cursor)] == CC_DIGIT);
                bool isFp = false;
                if (UNLIKELY(*cursor == '.')) {
                    if (lut[static_cast<uint8_t>(cursor[1])] == CC_DIGIT) {
                        isFp = true;
                        cursor++;
                        while (lut[static_cast<uint8_t>(*++cursor)] == CC_DIGIT);
                    }
                }
                lastTy = isFp ? TOKEN_LIT_FLOAT : TOKEN_LIT_INT;
                *out++ = {lastTy, llvm::StringRef(mark, cursor - mark), 0, 0};
            } else {
                lastTy = (cc == CC_PLUS) ? TOKEN_PLUS : TOKEN_MINUS;
                *out++ = {lastTy, (cc == CC_PLUS) ? "+" : "-", 0, 0};
                cursor++;
            }
            break;
        }
        case CC_EQ: {
            if (cursor[1] == '=') {
                *out++ = {TOKEN_EQUAL_EQUAL, "==", 0, 0};
                lastTy = TOKEN_EQUAL_EQUAL;
                cursor += 2;
            } else {
                *out++ = {TOKEN_EQUAL, "=", 0, 0};
                lastTy = TOKEN_EQUAL;
                cursor++;
            }
            break;
        }
        case CC_BANG: {
            if (cursor[1] == '=') {
                *out++ = {TOKEN_NOT_EQUAL, "!=", 0, 0};
                lastTy = TOKEN_NOT_EQUAL;
                cursor += 2;
            } else {
                *out++ = {TOKEN_NOT, "!", 0, 0};
                lastTy = TOKEN_NOT;
                cursor++;
            }
            break;
        }
        case CC_LT: {
            if (cursor[1] == '=') {
                *out++ = {TOKEN_LESS_EQUAL, "<=", 0, 0};
                lastTy = TOKEN_LESS_EQUAL;
                cursor += 2;
            } else {
                *out++ = {TOKEN_LESS, "<", 0, 0};
                lastTy = TOKEN_LESS;
                cursor++;
            }
            break;
        }
        case CC_GT: {
            if (cursor[1] == '=') {
                *out++ = {TOKEN_GREATER_EQUAL, ">=", 0, 0};
                lastTy = TOKEN_GREATER_EQUAL;
                cursor += 2;
            } else {
                *out++ = {TOKEN_GREATER, ">", 0, 0};
                lastTy = TOKEN_GREATER;
                cursor++;
            }
            break;
        }
        case CC_AMP: {
            if (cursor[1] == '&') {
                *out++ = {TOKEN_AND, "&&", 0, 0};
                lastTy = TOKEN_AND;
                cursor += 2;
            } else cursor++;
            break;
        }
        case CC_PIPE: {
            if (cursor[1] == '|') {
                *out++ = {TOKEN_OR, "||", 0, 0};
                lastTy = TOKEN_OR;
                cursor += 2;
            } else cursor++;
            break;
        }
        case CC_COLON: {
            if (cursor[1] == ':') {
                *out++ = {TOKEN_COLON, "::", 0, 0};
                lastTy = TOKEN_COLON;
                cursor += 2;
            } else {
                *out++ = {TOKEN_COLON, ":", 0, 0};
                lastTy = TOKEN_COLON;
                cursor++;
            }
            break;
        }
        case CC_DOT: *out++ = {TOKEN_DOT, ".", 0, 0}; lastTy = TOKEN_DOT; cursor++; break;
        case CC_STAR: *out++ = {TOKEN_STAR, "*", 0, 0}; lastTy = TOKEN_STAR; cursor++; break;
        case CC_PERCENT: *out++ = {TOKEN_MODULO, "%", 0, 0}; lastTy = TOKEN_MODULO; cursor++; break;
        case CC_LPAREN: *out++ = {TOKEN_PAREN_OPEN, "(", 0, 0}; lastTy = TOKEN_PAREN_OPEN; cursor++; break;
        case CC_RPAREN: *out++ = {TOKEN_PAREN_CLOSE, ")", 0, 0}; lastTy = TOKEN_PAREN_CLOSE; cursor++; break;
        case CC_LBRACE: *out++ = {TOKEN_BRACE_OPEN, "{", 0, 0}; lastTy = TOKEN_BRACE_OPEN; cursor++; break;
        case CC_RBRACE: *out++ = {TOKEN_BRACE_CLOSE, "}", 0, 0}; lastTy = TOKEN_BRACE_CLOSE; cursor++; break;
        case CC_LBRACK: *out++ = {TOKEN_BRACKET_OPEN, "[", 0, 0}; lastTy = TOKEN_BRACKET_OPEN; cursor++; break;
        case CC_RBRACK: *out++ = {TOKEN_BRACKET_CLOSE, "]", 0, 0}; lastTy = TOKEN_BRACKET_CLOSE; cursor++; break;
        case CC_SEMI: *out++ = {TOKEN_SEMICOLON, ";", 0, 0}; lastTy = TOKEN_SEMICOLON; cursor++; break;
        case CC_COMMA: *out++ = {TOKEN_COMMA, ",", 0, 0}; lastTy = TOKEN_COMMA; cursor++; break;
        default: 
            if (UNLIKELY(c == '\0')) goto end_lexing;
            cursor++; 
            break;
        }
    }

end_lexing:
    *out++ = {TOKEN_EOF, "", 0, 0};
    tokens.resize(out - tokens.data());

    // Lazy evaluation pass for lines and columns
    int currentLine = 1;
    const char* lineStart = sourceCode.data();
    const char* p = sourceCode.data();
    const char* endPtr = sourceCode.data() + sourceCode.size();
    
    for (auto& t : tokens) {
        if (t.type == TOKEN_EOF) {
            t.line = currentLine;
            t.column = static_cast<int>(endPtr - lineStart) + 1;
            continue;
        }
        
        const char* tokenStart = t.value.data();
        while (p < tokenStart) {
            if (UNLIKELY(*p == '\n')) {
                currentLine++;
                lineStart = p + 1;
            }
            p++;
        }
        
        t.line = currentLine;
        t.column = static_cast<int>(tokenStart - lineStart) + 1;
    }

    return tokens;
}
