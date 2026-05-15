//===-- lexer.cpp -----------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//
// NOLINTBEGIN(cppcoreguidelines-pro-bounds-pointer-arithmetic)

#include "compiler/lexer/lexer.h"
#include "compiler/lexer/token_type.h"
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <array>
#include <llvm-18/llvm/ADT/StringRef.h>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

constexpr size_t SIZEOCTET2 = 2;
constexpr size_t SIZEOCTET3 = 3;
constexpr size_t SIZEOCTET4 = 4;
constexpr size_t SIZEOCTET5 = 5;
constexpr size_t SIZEOCTET6 = 6;
constexpr size_t SIZEOCTET8 = 8; 
constexpr size_t LONGMAXREGISTRY64BITS = 8;

constexpr size_t SHIFT_32_BITS = 32;
constexpr size_t SHIFT_48_BITS = 48;

constexpr size_t ASCII_TABLE_SIZE = 256;
constexpr size_t UNARY_LUT_SIZE = 128;

constexpr size_t TOKEN_RESERVE_DIVISOR = 3;
constexpr size_t TOKEN_RESERVE_OFFSET = 4;

// Utility to convert a string of 1 to 8 characters into a uint64_t (Little Endian)
constexpr auto Lexer::to_kw(std::string_view str) -> uint64_t {

    if (str.size() > LONGMAXREGISTRY64BITS) {
         throw std::invalid_argument("Mot-clé trop long pour un registre 64 bits (max 8 caractères)");
    }
    uint64_t val = 0;
    for (std::size_t i = 0; i < str.size(); ++i) {
        val |= (static_cast<uint64_t>(str[i]) << (SIZEOCTET8 * i));
    }
    return val;
}

// Branch prediction hints
#define PRYSMA_UNLIKELY(x) __builtin_expect(!!(x), 0)

// Fast word-sized loads (assumes Little Endian)
namespace {
    inline auto ld16(const char* ptr) -> uint16_t { uint16_t value = 0; std::memcpy(&value, ptr, SIZEOCTET2); return value; }
    inline auto ld32(const char* ptr) -> uint32_t { uint32_t value = 0; std::memcpy(&value, ptr, SIZEOCTET4); return value; }
    inline auto ld64(const char* ptr) -> uint64_t { uint64_t value = 0; std::memcpy(&value, ptr, SIZEOCTET8); return value; }
}

namespace {
    constexpr auto classOf(unsigned char chr) -> uint8_t {
    if (chr >= 'a' && chr <= 'z') {return CC_ALPHA;}
    if (chr >= 'A' && chr <= 'Z') {return CC_ALPHA;}
    if (chr == '_')               {return CC_ALPHA;}
    if (chr >= '0' && chr <= '9') {return CC_DIGIT;}
    switch (chr) {
        case '.':  {return CC_DOT;     } 
        case '"':  {return CC_DQUOTE;  }
        case '/':  {return CC_SLASH;   } 
        case '*':  {return CC_STAR;    } 
        case '+':  {return CC_PLUS;    }
        case '-':  {return CC_MINUS;   } 
        case '%':  {return CC_PERCENT; } 
        case '=':  {return CC_EQ;      } 
        case '!':  {return CC_BANG;    }
        case '<':  {return CC_LT;      }
        case '>':  {return CC_GT;      }
        case '&':  {return CC_AMP;     }
        case '|':  {return CC_PIPE;    }
        case ':':  {return CC_COLON;   }
        case '(':  {return CC_LPAREN;  }
        case ')':  {return CC_RPAREN;  }
        case '{':  {return CC_LBRACE;  }
        case '}':  {return CC_RBRACE;  }
        case '[':  {return CC_LBRACK;  }
        case ']':  {return CC_RBRACK;  }
        case ';':  {return CC_SEMI;    }
        case ',':  {return CC_COMMA;   }
        case '\n': {return CC_NL;      }
        case ' ':
        case '\t':
        case '\r': {return CC_WS;      }
        default:   {return CC_OTHER;   }
        }
    }
}


// Template metaprogramming: Compile-time generation of the CharClass LUT

// TODO : template qui utilise une encienne façon de faire, il y a forcément une façon plus récente sans perte de performance 
namespace {
    template <std::size_t... Is>
    constexpr auto make_cclut(std::index_sequence<Is...> seq [[maybe_unused]]) -> std::array<uint8_t, ASCII_TABLE_SIZE> { 
        return {{ classOf(static_cast<unsigned char>(Is))... }};
    }
    constexpr auto cclut_array = make_cclut(std::make_index_sequence<ASCII_TABLE_SIZE>{});
}
   
// Keyword resolution via masked word-sized loads
// NOLINTNEXTLINE(readability-function-cognitive-complexity)
auto Lexer::resolveKeyword(llvm::StringRef word) -> TokenType {
    const char *str = word.data();
    // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)
    switch (word.size()) {
    case 2: {
        uint16_t value = ld16(str);
        if (value == to_kw("fn")) {return TOKEN_FUNCTION; }
        if (value == to_kw("if")) {return TOKEN_IF; }
        break;
    }
    case 3: {
        uint32_t value = 0; std::memcpy(&value, str, SIZEOCTET3);
        if (value == to_kw("for")) {return TOKEN_FOR;}   
        if (value == to_kw("dec")) {return TOKEN_DECL;}    
        if (value == to_kw("aff")) {return TOKEN_ASSIGN;}   
        if (value == to_kw("ref")) {return TOKEN_REF;}     
        if (value == to_kw("arg")) {return TOKEN_ARG;}   
        if (value == to_kw("new")) {return TOKEN_NEW;}    
        if (value == to_kw("ptr")) {return TOKEN_TYPE_PTR;} 
        break;
    }
    case 4: {
        uint32_t value = ld32(str);
        if (value == to_kw("else")) {return TOKEN_ELSE;}      
        if (value == to_kw("true")) {return TOKEN_TRUE;}      
        if (value == to_kw("void")) {return TOKEN_TYPE_VOID;} 
        if (value == to_kw("bool")) {return TOKEN_TYPE_BOOL;} 
        if (value == to_kw("char")) {return TOKEN_TYPE_CHAR;}
        if (value == to_kw("call")) {return TOKEN_CALL;}     
        if (value == to_kw("pass")) {return TOKEN_PASS;}     
        break;
    }
    case 5: {
        uint64_t value = 0; std::memcpy(&value, str, SIZEOCTET5);
        if (value == to_kw("while")) {return TOKEN_WHILE;}    
        if (value == to_kw("false")) {return TOKEN_FALSE;}     
        if (value == to_kw("scope")) {return TOKEN_SCOPE;}     
        if (value == to_kw("unref")) {return TOKEN_UNREF;}     
        if (value == to_kw("int64")) {return TOKEN_TYPE_INT64;}
        if (value == to_kw("int32")) {return TOKEN_TYPE_INT32;}
        if (value == to_kw("float")) {return TOKEN_TYPE_FLOAT;}
        if (value == to_kw("class")) {return TOKEN_CLASS;}
        break;
    }
    case 6: {
        uint64_t value6 = 0; std::memcpy(&value6, str, SIZEOCTET6);
        if (value6 == to_kw("return")) {return TOKEN_RETURN;}     
        if (value6 == to_kw("string")) {return TOKEN_TYPE_STRING;}
        if (value6 == to_kw("delete")) {return TOKEN_DELETE;}     
        if (value6 == to_kw("public")) {return TOKEN_PUBLIC;}     
        break;
    }
    case 7: {
        uint64_t value7 = ld32(str) | (static_cast<uint64_t>(ld16(str+SIZEOCTET4)) << SHIFT_32_BITS) | (static_cast<uint64_t>(str[SIZEOCTET6]) << SHIFT_48_BITS);
        if (value7 == to_kw("include")) {return TOKEN_INCLUDE;} 
        if (value7 == to_kw("private")) {return TOKEN_PRIVATE;} 
        break;
    }
    case 9: {
        if (ld64(str) == to_kw("protecte") && str[SIZEOCTET8] == 'd'){return TOKEN_PROTECTED;}
        break;
    }
    
    default:
        break;
    }
    // NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
    return TOKEN_IDENTIFIER;
}

namespace  {
    constexpr auto checkUnary(size_t typ) -> bool {
        switch (typ) {
            case TOKEN_EQUAL: case TOKEN_PAREN_OPEN: case TOKEN_COMMA:
            case TOKEN_BRACKET_OPEN: case TOKEN_PLUS: case TOKEN_MINUS:
            case TOKEN_STAR: case TOKEN_SLASH: case TOKEN_MODULO:
            case TOKEN_EQUAL_EQUAL: case TOKEN_NOT_EQUAL:
            case TOKEN_LESS: case TOKEN_GREATER:
            case TOKEN_LESS_EQUAL: case TOKEN_GREATER_EQUAL:
            case TOKEN_AND: case TOKEN_OR: case TOKEN_EOF:
            case TOKEN_BRACE_OPEN: case TOKEN_SEMICOLON: case TOKEN_COLON:
                return true;
            default:
                return false;
        }
    }
}
// TODO : template qui utilise une encienne façon de faire, il y a forcément une façon plus récente sans perte de performance 
// Template metaprogramming: Compile-time generation of the Unary Context LUT
namespace {
template <std::size_t... Is>
    constexpr auto make_unary_lut(std::index_sequence<Is...> /*unused*/) -> std::array<bool, UNARY_LUT_SIZE> {
        return {{ checkUnary(Is)... }};
    }
    constexpr auto unary_lut = make_unary_lut(std::make_index_sequence<UNARY_LUT_SIZE>{});
}

namespace {
    inline auto isUnary(TokenType typ) -> bool {
        return unary_lut.at(static_cast<size_t>(typ));
    }
}
 
// NOLINTNEXTLINE(readability-function-cognitive-complexity)
auto Lexer::tokenize(const string& sourceCode) -> vector<Token> {
    vector<Token> tokens;
    tokens.reserve((sourceCode.size() / TOKEN_RESERVE_DIVISOR) + TOKEN_RESERVE_OFFSET);

#define PUSH_TOKEN(typ, val) tokens.push_back({typ, val, 0, 0})

    const char* __restrict cursor = sourceCode.data();
    const uint8_t* __restrict lut = cclut_array.data();
    TokenType lastTy = TOKEN_EOF;

    while (true) {
        auto chr = static_cast<uint8_t>(*cursor);
        uint8_t chrc = lut[chr];

        switch (chrc) {
        case CC_WS:
        case CC_NL:
            cursor++;
            break;
        case CC_ALPHA: {
            const char* mark = cursor;
            while (true) {
                uint8_t nextClass = lut[static_cast<uint8_t>(*++cursor)];
                if (nextClass != CC_ALPHA && nextClass != CC_DIGIT)
                {
                    break;
                } 
            }
            llvm::StringRef word(mark, static_cast<size_t>(cursor - mark));
            TokenType typ = resolveKeyword(word);
            if (typ == TOKEN_TRUE)       { PUSH_TOKEN(TOKEN_LIT_BOOL, "1"); lastTy = TOKEN_LIT_BOOL; }
            else if (typ == TOKEN_FALSE) { PUSH_TOKEN(TOKEN_LIT_BOOL, "0"); lastTy = TOKEN_LIT_BOOL; }
            else                         { PUSH_TOKEN(typ, word); lastTy = typ; }
            break;
        }
        case CC_DIGIT: {
            const char* mark = cursor;
            while (lut[static_cast<uint8_t>(*++cursor)] == CC_DIGIT){};
            bool isFp = false;
            if (*cursor == '.') {
                if (lut[static_cast<uint8_t>(cursor[1])] == CC_DIGIT) {
                    isFp = true;
                    cursor++;
                    while (lut[static_cast<uint8_t>(*++cursor)] == CC_DIGIT){};
                }
            }
            lastTy = isFp ? TOKEN_LIT_FLOAT : TOKEN_LIT_INT;
            PUSH_TOKEN(lastTy, llvm::StringRef(mark, static_cast<size_t>(cursor - mark)));
            break;
        }
        case CC_DQUOTE: {
            PUSH_TOKEN(TOKEN_QUOTE, "\"");
            cursor++;
            const char* mark = cursor;
            while (true) {
                char chr = *cursor;
                if (PRYSMA_UNLIKELY(chr == '"' || chr == '\\' || chr == '\n' || chr == '\0')) {
                    if (chr == '\\' && cursor[1] != '\0') {
                        cursor += 2;
                    } else if (chr == '\n') {
                        cursor++;
                    } else {
                        break;
                    }
                } else {
                    cursor++;
                }
            }
            if (cursor > mark) {
                PUSH_TOKEN(TOKEN_IDENTIFIER, llvm::StringRef(mark, static_cast<size_t>(cursor - mark)));
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
                while (*cursor != '\n' && *cursor != '\0') {
                    cursor++;
                }
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
                while (lut[static_cast<uint8_t>(*++cursor)] == CC_DIGIT){};
                bool isFp = false;
                if (*cursor == '.') {
                    if (lut[static_cast<uint8_t>(cursor[1])] == CC_DIGIT) {
                        isFp = true;
                        cursor++;
                        while (lut[static_cast<uint8_t>(*++cursor)] == CC_DIGIT){};
                    }
                }
                lastTy = isFp ? TOKEN_LIT_FLOAT : TOKEN_LIT_INT;
                PUSH_TOKEN(lastTy, llvm::StringRef(mark, static_cast<size_t>(cursor - mark)));
            } else {
                lastTy = (chrc == CC_PLUS) ? TOKEN_PLUS : TOKEN_MINUS;
                PUSH_TOKEN(lastTy, (chrc == CC_PLUS) ? "+" : "-");
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
            } else {
                PUSH_TOKEN(TOKEN_AMP, "&");
                lastTy = TOKEN_AMP;
                cursor++;
            }
            break;
        }
        case CC_PIPE: {
            if (cursor[1] == '|') {
                PUSH_TOKEN(TOKEN_OR, "||");
                lastTy = TOKEN_OR;
                cursor += 2;
            } else {
                PUSH_TOKEN(TOKEN_PIPE, "|");
                lastTy = TOKEN_PIPE;
                cursor++;
            }
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
            if (PRYSMA_UNLIKELY(chr == '\0'))
            {
                 goto end_lexing;
            }
            PUSH_TOKEN(TOKEN_INVALID, llvm::StringRef(cursor, 1));
            lastTy = TOKEN_INVALID;
            cursor++; 
            break;
        }
    }

end_lexing:
    // TODO, line et column reste toujours à zéro petit problème, mais les optimisations pour le DFA sont incroyable
    tokens.push_back({TOKEN_EOF, "", 0, 0});
    
#undef PUSH_TOKEN
    return tokens;
}
// NOLINTEND(cppcoreguidelines-pro-bounds-pointer-arithmetic)