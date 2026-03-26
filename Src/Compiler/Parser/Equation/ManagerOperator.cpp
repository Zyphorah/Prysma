#include "Compiler/Parser/Equation/ManagerOperator.h"
#include "Compiler/Parser/Equation/Interfaces/IManagerOperator.h"
#include "Compiler/Parser/Equation/Interfaces/IManagerParenthesis.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
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
