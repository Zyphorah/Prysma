#include "Compiler/Parser/Equation/ChainOfResponsibility.h"
#include "Compiler/Parser/Equation/ManagerOperator.h"
#include "Compiler/Parser/Equation/Interfaces/IManagerParenthesis.h"
#include "Compiler/Lexer/Lexer.h"
#include <cstddef>
#include <utility>
#include <vector>

ChainOfResponsibility::ChainOfResponsibility(
    IManagerParenthese* managerParenthese, 
    std::vector<ManagerOperateur*> operateurs)
    : _debut(nullptr), _managerParenthese(managerParenthese), _operateurs(std::move(operateurs)) {
    
    // Chaîner les managers
    for (size_t i = 0; i < _operateurs.size(); i++) {
        _operateurs[i]->definirManagerParenthese(_managerParenthese);
        if (i < _operateurs.size() - 1) {
            _operateurs[i]->definirSuivant(_operateurs[i + 1]);
        }
    }
    
    if (!_operateurs.empty()) {
        _debut = _operateurs[0];
    }
}

ChainOfResponsibility::~ChainOfResponsibility() = default;

auto ChainOfResponsibility::trouverOperateur(const std::vector<Token>& equation) const -> int {
    if (_debut == nullptr) {
        return -1;
    }
    return _debut->traiter(equation);
}
