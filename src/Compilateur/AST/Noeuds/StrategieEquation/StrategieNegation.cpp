#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieNegation.h"
#include "Compilateur/AST/Noeuds/Operande/NoeudNegation.h"
#include <stdexcept>

std::shared_ptr<INoeud> StrategieNegation::construire(std::vector<Token>& equation) {
    if (equation.size() < 2) {
        throw std::runtime_error("Erreur: '!' doit être suivi d'une expression");
    }
    std::vector<Token> operande(equation.begin() + 1, equation.end());
    std::shared_ptr<INoeud> exprOperande = std::make_shared<NoeudNegation>(equation[0], nullptr);
    return exprOperande;
}
