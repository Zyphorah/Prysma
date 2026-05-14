//===-- lexer.cpp -----------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#include "compiler/lexer/lexer.h"
#include "compiler/lexer/token_type.h"
#include <cstdint>
#include <cstring>
#include <array>
#include <utility>

// Branch prediction hints
#define UNLIKELY(x) __builtin_expect(!!(x), 0)

// Fast word-sized loads (assumes Little Endian)
static inline auto ld16(const char* p) -> uint16_t { uint16_t v; std::memcpy(&v, p, 2); return v; }
static inline auto ld32(const char* p) -> uint32_t { uint32_t v; std::memcpy(&v, p, 4); return v; }
static inline auto ld64(const char* p) -> uint64_t { uint64_t v; std::memcpy(&v, p, 8); return v; }

static constexpr auto classOf(unsigned char c) -> uint8_t {
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

// Template metaprogramming: Compile-time generation of the CharClass LUT
template <std::size_t... Is>
constexpr auto make_cclut(std::index_sequence<Is...>) -> std::array<uint8_t, 256> {
    return {{ classOf(static_cast<unsigned char>(Is))... }};
}
constexpr auto cclut_array = make_cclut(std::make_index_sequence<256>{});

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
        uint64_t v = ld32(s) | (static_cast<uint64_t>(ld16(s+4)) << 32) | (static_cast<uint64_t>(s[6]) << 48);
        if (v == 0x6564756C636E69ULL) return TOKEN_INCLUDE; // "include"
        if (v == 0x65746176697270ULL) return TOKEN_PRIVATE; // "private"
        break;
    }
    case 9: {
        if (ld64(s) == 0x65746365746F7270ULL && s[8] == 'd') return TOKEN_PROTECTED; // "protected"
        break;
    }
    }
    return TOKEN_IDENTIFIER;
}

constexpr bool checkUnary(size_t t) {
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

// Template metaprogramming: Compile-time generation of the Unary Context LUT
template <std::size_t... Is>
constexpr auto make_unary_lut(std::index_sequence<Is...>) -> std::array<bool, 128> {
    return {{ checkUnary(Is)... }};
}
constexpr auto unary_lut = make_unary_lut(std::make_index_sequence<128>{});

static inline bool isUnary(TokenType t) {
    return unary_lut[static_cast<size_t>(t)];
}

#include <algorithm>

auto Lexer::tokenize(const string& sourceCode) -> vector<Token> {
    size_t capacity = sourceCode.size() / 3 + 4;
    Token* tokensData = static_cast<Token*>(std::malloc(capacity * sizeof(Token)));
    Token* out = tokensData;
    Token* outEnd = tokensData + capacity - 1; // reserve 1 for EOF

#define PUSH_TOKEN(ty, val) \
    do { \
        if (UNLIKELY(out >= outEnd)) { \
            size_t sz = out - tokensData; \
            capacity *= 2; \
            tokensData = static_cast<Token*>(std::realloc(tokensData, capacity * sizeof(Token))); \
            out = tokensData + sz; \
            outEnd = tokensData + capacity - 1; \
        } \
        *out++ = {ty, val, 0, 0}; \
    } while(0)

    const char* __restrict cursor = sourceCode.data();
    const uint8_t* __restrict lut = cclut_array.data();
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
            if (ty == TOKEN_TRUE)       { PUSH_TOKEN(TOKEN_LIT_BOOL, "1"); lastTy = TOKEN_LIT_BOOL; }
            else if (ty == TOKEN_FALSE) { PUSH_TOKEN(TOKEN_LIT_BOOL, "0"); lastTy = TOKEN_LIT_BOOL; }
            else                        { PUSH_TOKEN(ty, word); lastTy = ty; }
            break;
        }
        case CC_DIGIT: {
            const char* mark = cursor;
            while (lut[static_cast<uint8_t>(*++cursor)] == CC_DIGIT);
            bool isFp = false;
            if (*cursor == '.') {
                if (lut[static_cast<uint8_t>(cursor[1])] == CC_DIGIT) {
                    isFp = true;
                    cursor++;
                    while (lut[static_cast<uint8_t>(*++cursor)] == CC_DIGIT);
                }
            }
            lastTy = isFp ? TOKEN_LIT_FLOAT : TOKEN_LIT_INT;
            PUSH_TOKEN(lastTy, llvm::StringRef(mark, cursor - mark));
            break;
        }
        case CC_DQUOTE: {
            PUSH_TOKEN(TOKEN_QUOTE, "\"");
            cursor++;
            const char* mark = cursor;
            while (true) {
                char ch = *cursor;
                if (UNLIKELY(ch == '"' || ch == '\\' || ch == '\n' || ch == '\0')) {
                    if (ch == '\\' && cursor[1] != '\0') {
                        cursor += 2;
                    } else if (ch == '\n') {
                        cursor++;
                    } else {
                        break;
                    }
                } else {
                    cursor++;
                }
            }
            if (cursor > mark) {
                PUSH_TOKEN(TOKEN_IDENTIFIER, llvm::StringRef(mark, cursor - mark));
            }
            if (*cursor == '"') {
                PUSH_TOKEN(TOKEN_QUOTE, "\"");
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
            PUSH_TOKEN(TOKEN_SLASH, "/");
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
                if (*cursor == '.') {
                    if (lut[static_cast<uint8_t>(cursor[1])] == CC_DIGIT) {
                        isFp = true;
                        cursor++;
                        while (lut[static_cast<uint8_t>(*++cursor)] == CC_DIGIT);
                    }
                }
                lastTy = isFp ? TOKEN_LIT_FLOAT : TOKEN_LIT_INT;
                PUSH_TOKEN(lastTy, llvm::StringRef(mark, cursor - mark));
            } else {
                lastTy = (cc == CC_PLUS) ? TOKEN_PLUS : TOKEN_MINUS;
                PUSH_TOKEN(lastTy, (cc == CC_PLUS) ? "+" : "-");
                cursor++;
            }
            break;
        }
        case CC_EQ: {
            if (cursor[1] == '=') {
                PUSH_TOKEN(TOKEN_EQUAL_EQUAL, "==");
                lastTy = TOKEN_EQUAL_EQUAL;
                cursor += 2;
            } else {
                PUSH_TOKEN(TOKEN_EQUAL, "=");
                lastTy = TOKEN_EQUAL;
                cursor++;
            }
            break;
        }
        case CC_BANG: {
            if (cursor[1] == '=') {
                PUSH_TOKEN(TOKEN_NOT_EQUAL, "!=");
                lastTy = TOKEN_NOT_EQUAL;
                cursor += 2;
            } else {
                PUSH_TOKEN(TOKEN_NOT, "!");
                lastTy = TOKEN_NOT;
                cursor++;
            }
            break;
        }
        case CC_LT: {
            if (cursor[1] == '=') {
                PUSH_TOKEN(TOKEN_LESS_EQUAL, "<=");
                lastTy = TOKEN_LESS_EQUAL;
                cursor += 2;
            } else {
                PUSH_TOKEN(TOKEN_LESS, "<");
                lastTy = TOKEN_LESS;
                cursor++;
            }
            break;
        }
        case CC_GT: {
            if (cursor[1] == '=') {
                PUSH_TOKEN(TOKEN_GREATER_EQUAL, ">=");
                lastTy = TOKEN_GREATER_EQUAL;
                cursor += 2;
            } else {
                PUSH_TOKEN(TOKEN_GREATER, ">");
                lastTy = TOKEN_GREATER;
                cursor++;
            }
            break;
        }
        case CC_AMP: {
            if (cursor[1] == '&') {
                PUSH_TOKEN(TOKEN_AND, "&&");
                lastTy = TOKEN_AND;
                cursor += 2;
            } else cursor++;
            break;
        }
        case CC_PIPE: {
            if (cursor[1] == '|') {
                PUSH_TOKEN(TOKEN_OR, "||");
                lastTy = TOKEN_OR;
                cursor += 2;
            } else cursor++;
            break;
        }
        case CC_COLON: {
            if (cursor[1] == ':') {
                PUSH_TOKEN(TOKEN_COLON, "::");
                lastTy = TOKEN_COLON;
                cursor += 2;
            } else {
                PUSH_TOKEN(TOKEN_COLON, ":");
                lastTy = TOKEN_COLON;
                cursor++;
            }
            break;
        }
        case CC_DOT: PUSH_TOKEN(TOKEN_DOT, "."); lastTy = TOKEN_DOT; cursor++; break;
        case CC_STAR: PUSH_TOKEN(TOKEN_STAR, "*"); lastTy = TOKEN_STAR; cursor++; break;
        case CC_PERCENT: PUSH_TOKEN(TOKEN_MODULO, "%"); lastTy = TOKEN_MODULO; cursor++; break;
        case CC_LPAREN: PUSH_TOKEN(TOKEN_PAREN_OPEN, "("); lastTy = TOKEN_PAREN_OPEN; cursor++; break;
        case CC_RPAREN: PUSH_TOKEN(TOKEN_PAREN_CLOSE, ")"); lastTy = TOKEN_PAREN_CLOSE; cursor++; break;
        case CC_LBRACE: PUSH_TOKEN(TOKEN_BRACE_OPEN, "{"); lastTy = TOKEN_BRACE_OPEN; cursor++; break;
        case CC_RBRACE: PUSH_TOKEN(TOKEN_BRACE_CLOSE, "}"); lastTy = TOKEN_BRACE_CLOSE; cursor++; break;
        case CC_LBRACK: PUSH_TOKEN(TOKEN_BRACKET_OPEN, "["); lastTy = TOKEN_BRACKET_OPEN; cursor++; break;
        case CC_RBRACK: PUSH_TOKEN(TOKEN_BRACKET_CLOSE, "]"); lastTy = TOKEN_BRACKET_CLOSE; cursor++; break;
        case CC_SEMI: PUSH_TOKEN(TOKEN_SEMICOLON, ";"); lastTy = TOKEN_SEMICOLON; cursor++; break;
        case CC_COMMA: PUSH_TOKEN(TOKEN_COMMA, ","); lastTy = TOKEN_COMMA; cursor++; break;
        default: 
            if (UNLIKELY(c == '\0')) goto end_lexing;
            cursor++; 
            break;
        }
    }

end_lexing:
    *out++ = {TOKEN_EOF, "", 0, 0};
    
    // Construct vector from raw array and free it
    vector<Token> tokens(tokensData, out);
    std::free(tokensData);
#undef PUSH_TOKEN
    return tokens;
}
