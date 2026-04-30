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
#include <vector>

using namespace std;

struct Token {
    TokenType type;
    llvm::StringRef value;
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
    static TokenType resolveKeyword(llvm::StringRef word);
};

#endif /* F2D02E97_AF62_409B_84AD_90905E9BE240 */
