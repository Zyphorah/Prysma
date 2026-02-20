#include "Compilateur/AnalyseSyntaxique/Equation/ServiceParenthese.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/AST/Registre/RegistreSymbole.h"
#include <utility>

ServiceParenthese::ServiceParenthese(RegistreSymbole* registreSymbole)
    : _registreSymbole(registreSymbole) {
}

// TODO: OPTIMISATION utilisation de int debut et int fin pour éviter la copie du vecteur de token et ne rien retourner
std::vector<Token> ServiceParenthese::enleverParenthesesEnglobantes(const vector<Token>& equation) {
    std::vector<Token> result = equation;
    
    // Enlever les tokens EOF au début et à la fin
    while (!result.empty() && result.front().type == TOKEN_EOF) {
        result.erase(result.begin());
    }
    while (!result.empty() && result.back().type == TOKEN_EOF) {
        result.erase(result.end() - 1);
    }
    
    while (result.size() >= 2 && 
           result.front().type == TOKEN_PAREN_OUVERTE && 
           result.back().type == TOKEN_PAREN_FERMEE && 
           estEnglobante(result)) {
        result.erase(result.begin());
        result.erase(result.end() - 1);
    }
    return result;
}

bool ServiceParenthese::estEnglobante(const vector<Token>& equation) {
    if (equation.size() < 2) {
        return false;
    }
    
    if (equation.front().type != TOKEN_PAREN_OUVERTE) {
        return false;
    }

    if (equation.back().type != TOKEN_PAREN_FERMEE) {
        return false;
    }
    
    // Vérifier que la profondeur ne retombe pas à 0 avant le dernier token
    int profondeur = 0;
    for (size_t i = 0; i < equation.size(); i++) {
        Token token = equation[i];
        profondeur += (token.type == TOKEN_PAREN_OUVERTE ? 1 : token.type == TOKEN_PAREN_FERMEE ? -1 : 0);
        // Si on atteint profondeur 0 avant le dernier token, ce n'est pas englobant
        if (profondeur == 0 && i < equation.size() - 1) {
            return false;
        }
    }
    
    return profondeur == 0;
}

int ServiceParenthese::trouverDernierAuNiveauZero(const vector<Token>& equation, Token operateur) {
    int profondeur = 0;
    for (int i = equation.size() - 1; i >= 0; i--) {
        Token token = equation[i];
        profondeur += (token.type == TOKEN_PAREN_FERMEE ? 1 : token.type == TOKEN_PAREN_OUVERTE ? -1 : 0);
        
        if (token.type == operateur.type && profondeur == 0 && !estSigneUnaire(equation, i)) {
            if (i > 0 && operateur.type == equation[i - 1].type) {
                return i - 1;
            }
            return i;
        }
    }
    return -1;
}

bool ServiceParenthese::estSigneUnaire(const vector<Token>& equation, int indice) const {
    if (indice == 0) {
        return true;
    }
    Token precedent = equation[indice - 1];
    return _registreSymbole->estOperateur(precedent.type) || precedent.type == TOKEN_PAREN_OUVERTE;
}
