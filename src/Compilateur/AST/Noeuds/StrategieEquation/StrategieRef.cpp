#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieRef.h"
#include "Compilateur/AST/Noeuds/Variable/NoeudRefVariable.h"
#include "Compilateur/Lexer/TokenType.h"
#include <stdexcept>

INoeud* StrategieRef::construire(std::vector<Token>& equation) {
    if (equation.size() < 2 || equation[1].type != TOKEN_IDENTIFIANT) {
        throw std::runtime_error("Erreur: 'ref' doit être suivi d'un identifiant");
    }
    return new NoeudRefVariable(equation[1].value);
}
