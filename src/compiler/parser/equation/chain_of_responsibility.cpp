//===-- chain_of_responsibility.cpp -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#include "compiler/parser/equation/chain_of_responsibility.h"
#include "compiler/parser/equation/manager_operator.h"
#include "compiler/parser/equation/interfaces/i_manager_parenthesis.h"
#include "compiler/lexer/lexer.h"
#include <cstddef>
#include <llvm/ADT/SmallVector.h>
#include <utility>
#include <vector>

ChainOfResponsibility::ChainOfResponsibility(
    IManagerParenthesis* parenthesisManager, 
    llvm::SmallVector<OperatorManager*, MAX_OPERATORS> operators)
    : _start(nullptr), _parenthesisManager(parenthesisManager), _operators(std::move(operators)) {
    
    // Chain the managers
    for (size_t i = 0; i < _operators.size(); i++) {
        _operators[i]->setParenthesisManager(_parenthesisManager);
        if (i < _operators.size() - 1) {
            _operators[i]->setNext(_operators[i + 1]);
        }
    }
    
    if (!_operators.empty()) {
        _start = _operators[0];
    }
}

ChainOfResponsibility::~ChainOfResponsibility() = default;

auto ChainOfResponsibility::findOperator(const std::vector<Token>& equation) const -> int {
    if (_start == nullptr) {
        return -1;
    }
    return _start->handle(equation);
}
