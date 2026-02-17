#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieIdentifiant.h"
#include "Compilateur/AST/Noeuds/Variable/NoeudUnRefVariable.h"

std::shared_ptr<INoeud> StrategieIdentifiant::construire(std::vector<Token>& equation) {
    return std::make_shared<NoeudUnRefVariable>(equation[0].value);
}
