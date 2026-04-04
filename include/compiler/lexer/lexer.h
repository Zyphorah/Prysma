//===-- lexer.h -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#ifndef F2D02E97_AF62_409B_84AD_90905E9BE240
#define F2D02E97_AF62_409B_84AD_90905E9BE240
#include "token_type.h"
#include <array>
#include <cstddef>
#include <llvm-18/llvm/ADT/StringRef.h>
#include <string>
#include <utility>
#include <vector>

using namespace std;
// lexer used for tokenizing the prysma source code
// filters the source code into a list of identified tokens

struct Token {
    TokenType type;
    llvm::StringRef value;
    int line;
    int column;
};

class Lexer {
    private:

    // Dictionary for reserved keywords of the prysma programming language
    // Allows to differentiate identifiers from keywords
    static constexpr std::array<std::pair<const char*, TokenType>, 31> keywordsArray = {{
        {"char", TOKEN_TYPE_CHAR},
        {"arg", TOKEN_ARG},
        {"fn", TOKEN_FUNCTION},
        {"if", TOKEN_IF},
        {"else", TOKEN_ELSE},
        {"while", TOKEN_WHILE},
        {"for", TOKEN_FOR},
        {"return", TOKEN_RETURN},
        {"true", TOKEN_TRUE},
        {"false", TOKEN_FALSE},
        {"scope", TOKEN_SCOPE},
        {"aff", TOKEN_ASSIGN},
        {"dec", TOKEN_DECL},
        {"int64", TOKEN_TYPE_INT64},
        {"int32", TOKEN_TYPE_INT32},
        {"float", TOKEN_TYPE_FLOAT},
        {"string", TOKEN_TYPE_STRING},
        {"bool", TOKEN_TYPE_BOOL},
        {"void", TOKEN_TYPE_VOID},
        {"call", TOKEN_CALL},
        {"pass", TOKEN_PASS},
        {"ref", TOKEN_REF},
        {"unref", TOKEN_UNREF},
        {"include", TOKEN_INCLUDE},
        {"new", TOKEN_NEW},
        {"ptr",TOKEN_TYPE_PTR},
        {"delete", TOKEN_DELETE},
        {"class", TOKEN_CLASS},
        {"public", TOKEN_PUBLIC},
        {"private", TOKEN_PRIVATE},
        {"protected",TOKEN_PROTECTED},
    }};

    // Private functions for tokenization
    static void addCurrentWord(llvm::StringRef currentWord, vector<Token>& tokens, int line, int column);
    static void handleOperatorsAndDelimiters(char current, const string& sourceCode, size_t& pos, vector<Token>& tokens, int line, int& column);
    static void handleMathOperators(char current, vector<Token>& tokens, int line, int column);
    static void handleDelimiters(char current, vector<Token>& tokens, int line, int column);
    static void handleComplexOperators(char current, const string& sourceCode, size_t& pos, vector<Token>& tokens, int line, int& column);
    static void handleLiterals(char current, const string& sourceCode, size_t& pos, vector<Token>& tokens, int line, int& column);
    static void handleComments(const string& sourceCode, size_t& pos);
    static void handleNumber(const string& sourceCode, size_t& pos, vector<Token>& tokens, int line, int& column);
    static auto isNegativeNumberContext(const vector<Token>& tokens) -> bool;

    public:
    static auto tokenize(const string& sourceCode) -> vector<Token>;
};

#endif /* F2D02E97_AF62_409B_84AD_90905E9BE240 */
