#include "Compiler/Parser/Equation/ServiceParenthesis.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include "Compiler/AST/Registry/RegistrySymbole.h"
#include <cstddef>
#include <vector>

ServiceParenthese::ServiceParenthese(RegistrySymbole* registrySymbole)
    : _registrySymbole(registrySymbole) {
}

// TODO: OPTIMISATION utilisation de int debut et int fin pour éviter la copie du vecteur de token et ne rien returnner
auto ServiceParenthese::enleverParenthesesEnglobantes(const vector<Token>& equation) -> std::vector<Token> {
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

auto ServiceParenthese::estEnglobante(const vector<Token>& equation) -> bool {
    if (equation.size() < 2) {
        return false;
    }
    
    if (equation.front().type != TOKEN_PAREN_OUVERTE) {
        return false;
    }

    if (equation.back().type != TOKEN_PAREN_FERMEE) {
        return false;
    }
    
    // Vérifier que la depth ne retombe pas à 0 avant le dernier token
    int depth = 0;
    for (size_t i = 0; i < equation.size(); i++) {
        Token token = equation[i];
        depth += (token.type == TOKEN_PAREN_OUVERTE ? 1 : token.type == TOKEN_PAREN_FERMEE ? -1 : 0);
        // Si on atteint depth 0 avant le dernier token, ce n'est pas englobant
        if (depth == 0 && i < equation.size() - 1) {
            return false;
        }
    }
    
    return depth == 0;
}

auto ServiceParenthese::trouverDernierAuNiveauZero(const vector<Token>& equation, Token operateur) -> int {
    int depthParen = 0;
    int depthCrochet = 0;
    for (int i = static_cast<int>(equation.size()) - 1; i >= 0; i--) {
        Token token = equation[static_cast<size_t>(i)];
        depthParen += (token.type == TOKEN_PAREN_FERMEE ? 1 : token.type == TOKEN_PAREN_OUVERTE ? -1 : 0);
        depthCrochet += (token.type == TOKEN_CROCHET_FERME ? 1 : token.type == TOKEN_CROCHET_OUVERT ? -1 : 0);
        
        if (token.type == operateur.type && depthParen == 0 && depthCrochet == 0 && !estSigneUnaire(equation, i)) {
            if (i > 0 && operateur.type == equation[static_cast<size_t>(i - 1)].type) {
                return i - 1;
            }
            return i;
        }
    }
    return -1;
}

auto ServiceParenthese::estSigneUnaire(const vector<Token>& equation, int indice) const -> bool {
    if (indice == 0) {
        return true;
    }
    Token precedent = equation[static_cast<size_t>(indice - 1)];
    return _registrySymbole->estOperateur(precedent.type) || precedent.type == TOKEN_PAREN_OUVERTE;
}
