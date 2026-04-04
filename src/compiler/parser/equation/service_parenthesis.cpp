//===-- service_parenthesis.cpp -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#include "compiler/parser/equation/service_parenthesis.h"
#include "compiler/lexer/lexer.h"
#include "compiler/lexer/token_type.h"
#include "compiler/ast/registry/registry_symbole.h"
#include <cstddef>
#include <vector>

ParenthesisService::ParenthesisService(RegistrySymbol* _registrySymbol)
    : _registrySymbol(_registrySymbol) {
}

// TODO: OPTIMIZATION use int start and int end to avoid copying the token vector and not return anything
auto ParenthesisService::removeWrappingParentheses(const std::vector<Token>& equation) -> std::vector<Token> {
    std::vector<Token> result = equation;
    
    // Remove EOF tokens at the beginning and end
    while (!result.empty() && result.front().type == TOKEN_EOF) {
        result.erase(result.begin());
    }
    while (!result.empty() && result.back().type == TOKEN_EOF) {
        result.erase(result.end() - 1);
    }
    
    while (result.size() >= 2 && 
           result.front().type == TOKEN_PAREN_OPEN && 
           result.back().type == TOKEN_PAREN_CLOSE && 
           isWrapping(result)) {
        result.erase(result.begin());
        result.erase(result.end() - 1);
    }
    return result;
}

auto ParenthesisService::isWrapping(const std::vector<Token>& equation) -> bool {
    if (equation.size() < 2) {
        return false;
    }
    
    if (equation.front().type != TOKEN_PAREN_OPEN) {
        return false;
    }

    if (equation.back().type != TOKEN_PAREN_CLOSE) {
        return false;
    }
    
    // Check that the depth does not return to 0 before the last token
    int depth = 0;
    for (size_t i = 0; i < equation.size(); i++) {
        Token token = equation[i];
        depth += (token.type == TOKEN_PAREN_OPEN ? 1 : token.type == TOKEN_PAREN_CLOSE ? -1 : 0);
        // If we reach depth 0 before the last token, it's not wrapping
        if (depth == 0 && i < equation.size() - 1) {
            return false;
        }
    }
    
    return depth == 0;
}

auto ParenthesisService::findLastAtLevelZero(const std::vector<Token>& equation, Token op) -> int {
    int depthParen = 0;
    int depthBracket = 0;
    for (int i = static_cast<int>(equation.size()) - 1; i >= 0; i--) {
        Token token = equation[static_cast<size_t>(i)];
        depthParen += (token.type == TOKEN_PAREN_CLOSE ? 1 : token.type == TOKEN_PAREN_OPEN ? -1 : 0);
        depthBracket += (token.type == TOKEN_BRACKET_CLOSE ? 1 : token.type == TOKEN_BRACKET_OPEN ? -1 : 0);
        
        if (token.type == op.type && depthParen == 0 && depthBracket == 0 && !isUnarySign(equation, i)) {
            if (i > 0 && op.type == equation[static_cast<size_t>(i - 1)].type) {
                return i - 1;
            }
            return i;
        }
    }
    return -1;
}

auto ParenthesisService::isUnarySign(const std::vector<Token>& equation, int index) const -> bool {
    if (index == 0) {
        return true;
    }
    Token previous = equation[static_cast<size_t>(index - 1)];
    return _registrySymbol->isOperator(previous.type) || previous.type == TOKEN_PAREN_OPEN;
}
