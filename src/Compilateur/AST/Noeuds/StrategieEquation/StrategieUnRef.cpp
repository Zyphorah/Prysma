#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieUnRef.h"
#include "Compilateur/AST/Noeuds/Variable/NoeudUnRefVariable.h"
#include "Compilateur/Lexer/TokenType.h"
#include <stdexcept>

std::shared_ptr<INoeud> StrategieUnRef::construire(std::vector<Token>& equation) {
    if (equation.size() < 2 || equation[1].type != TOKEN_IDENTIFIANT) {
        throw std::runtime_error("Erreur: 'unref' doit être suivi d'un identifiant");
    }
    return std::make_shared<NoeudUnRefVariable>(equation[1].value);
}
