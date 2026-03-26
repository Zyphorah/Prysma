#include "Compiler/Parser/Equation/ChainOfResponsibility.h"
#include "Compiler/Parser/Equation/ManagerOperator.h"
#include "Compiler/Parser/Equation/Interfaces/IManagerParenthesis.h"
#include "Compiler/Lexer/Lexer.h"
#include <cstddef>
#include <utility>
#include <vector>

ChainOfResponsibility::ChainOfResponsibility(
    IManagerParenthesis* parenthesisManager, 
    std::vector<OperatorManager*> operators)
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
