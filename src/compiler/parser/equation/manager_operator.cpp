//===-- manager_operator.cpp -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#include "compiler/parser/equation/manager_operator.h"
#include "compiler/parser/equation/interfaces/i_manager_operator.h"
#include "compiler/parser/equation/interfaces/i_manager_parenthesis.h"
#include "compiler/lexer/lexer.h"
#include "compiler/lexer/token_type.h"
#include <vector>

OperatorManager::OperatorManager(TokenType tokenType)
    : _next(nullptr), _parenthesisManager(nullptr), _tokenType(tokenType) {}

void OperatorManager::setParenthesisManager(IManagerParenthesis* parenthesisManager) {
    _parenthesisManager = parenthesisManager;
}

void OperatorManager::setNext(IOperatorManager* next) {
    _next = next;
}

auto OperatorManager::findOperator(const std::vector<Token>& equation) const -> int {
    if (_parenthesisManager == nullptr) {
        return -1;
    }
    Token op;
    op.type = _tokenType;
    op.value = "";
    return _parenthesisManager->findLastAtLevelZero(equation, op);
}

auto OperatorManager::handle(const std::vector<Token>& equation) -> int {
    int index = findOperator(equation);
    
    if (index != -1) {
        return index;
    }
    
    if (_next != nullptr) {
        return _next->handle(equation);
    }
    
    return -1;
}
