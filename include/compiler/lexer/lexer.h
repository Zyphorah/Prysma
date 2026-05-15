//===-- lexer.h -------------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//
//
// DFA-driven lexer for Prysma source files.
// Flat transition table indexed by (state, char-class) pairs.
// Character classes fold the 256-byte alphabet down so the hot
// data fits in L1. Keywords resolved post-scan via length-bucketed switch.
//
//===----------------------------------------------------------------------===//

#ifndef F2D02E97_AF62_409B_84AD_90905E9BE240
#define F2D02E97_AF62_409B_84AD_90905E9BE240

#include "token_type.h"
#include <cstdint>
#include <llvm-18/llvm/ADT/StringRef.h>
#include <string>
#include <string_view>
#include <vector>

using namespace std;

struct Token {
    TokenType type;
    llvm::StringRef value;

    // TODO : très bien mais il manque cette fonctionnalité, à réserver pour plus tard. 
    int line;
    int column;
};

enum CharClass : uint8_t {
    CC_OTHER = 0,
    CC_ALPHA,    // a-z A-Z _
    CC_DIGIT,    // 0-9
    CC_DOT,
    CC_DQUOTE,
    CC_SLASH,
    CC_STAR,
    CC_PLUS,
    CC_MINUS,
    CC_PERCENT,
    CC_EQ,
    CC_BANG,
    CC_LT,
    CC_GT,
    CC_AMP,
    CC_PIPE,
    CC_COLON,
    CC_LPAREN,
    CC_RPAREN,
    CC_LBRACE,
    CC_RBRACE,
    CC_LBRACK,
    CC_RBRACK,
    CC_SEMI,
    CC_COMMA,
    CC_NL,
    CC_WS,
    NUM_CLASSES
};

class Lexer {
public:
    static auto tokenize(const string& sourceCode) -> vector<Token>;

private:
    static auto resolveKeyword(llvm::StringRef word) -> TokenType;

    // This function transforms a string into a hexadecimal number at compile time.

    // Why do we do this? We perform this operation for performance reasons: instead of comparing two strings,
    // which takes a lot of time, we transform the string into a hexadecimal number at compile time.
    // The comparison then takes only a single CPU cycle instead of several, and cache locality is much better.
    // Here is an example:

    // i = 0 ('e' = 0x65): Shifted by 0 bits -> 0x00000065
    // i = 1 ('l' = 0x6C): Shifted by 8 bits -> 0x00006C00
    // i = 2 ('s' = 0x73): Shifted by 16 bits -> 0x00730000
    // i = 3 ('e' = 0x65): Shifted by 24 bits -> 0x65000000

    // So, instead of writing
    // if (value == 0x656C696877) { return TOKEN_WHILE; }
    // to directly compare a string transformed into hexadecimal,
    // we ask the CPU to do this transformation.
    // This gives us something more readable.
    // if (value == to_kw("while")) { return TOKEN_WHILE; }

    static constexpr auto to_kw(std::string_view str) -> uint64_t;
};

#endif /* F2D02E97_AF62_409B_84AD_90905E9BE240 */
