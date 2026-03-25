#include "Compiler/Parser/Equation/ManagerOperator.h"
#include "Compiler/Parser/Equation/Interfaces/IManagerOperator.h"
#include "Compiler/Parser/Equation/Interfaces/IManagerParenthesis.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include <vector>

ManagerOperateur::ManagerOperateur(TokenType tokenType)
    : _suivant(nullptr), _managerParenthese(nullptr), _tokenType(tokenType) {}

void ManagerOperateur::definirManagerParenthese(IManagerParenthese* managerParenthese) {
    _managerParenthese = managerParenthese;
}

void ManagerOperateur::definirSuivant(IManagerOperateur* suivant) {
    _suivant = suivant;
}

auto ManagerOperateur::chercherOperateur(const std::vector<Token>& equation) const -> int {
    if (_managerParenthese == nullptr) {
        return -1;
    }
    Token operateur;
    operateur.type = _tokenType;
    operateur.value = "";
    return _managerParenthese->trouverDernierAuNiveauZero(equation, operateur);
}

auto ManagerOperateur::traiter(const std::vector<Token>& equation) -> int {
    int indice = chercherOperateur(equation);
    
    if (indice != -1) {
        return indice;
    }
    
    if (_suivant != nullptr) {
        return _suivant->traiter(equation);
    }
    
    return -1;
}
