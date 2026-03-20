#include "Compilateur/AnalyseSyntaxique/Equation/GestionnaireOperateur.h"
#include "Compilateur/AnalyseSyntaxique/Equation/Interfaces/IGestionnaireOperateur.h"
#include "Compilateur/AnalyseSyntaxique/Equation/Interfaces/IGestionnaireParenthese.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Lexer/TokenType.h"
#include <vector>

GestionnaireOperateur::GestionnaireOperateur(TokenType tokenType)
    : _suivant(nullptr), _gestionnaireParenthese(nullptr), _tokenType(tokenType) {}

void GestionnaireOperateur::definirGestionnaireParenthese(IGestionnaireParenthese* gestionnaireParenthese) {
    _gestionnaireParenthese = gestionnaireParenthese;
}

void GestionnaireOperateur::definirSuivant(IGestionnaireOperateur* suivant) {
    _suivant = suivant;
}

auto GestionnaireOperateur::chercherOperateur(const std::vector<Token>& equation) const -> int {
    if (_gestionnaireParenthese == nullptr) {
        return -1;
    }
    Token operateur;
    operateur.type = _tokenType;
    operateur.value = "";
    return _gestionnaireParenthese->trouverDernierAuNiveauZero(equation, operateur);
}

auto GestionnaireOperateur::traiter(const std::vector<Token>& equation) -> int {
    int indice = chercherOperateur(equation);
    
    if (indice != -1) {
        return indice;
    }
    
    if (_suivant != nullptr) {
        return _suivant->traiter(equation);
    }
    
    return -1;
}
