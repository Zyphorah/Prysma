#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieLitteral.h"
#include "Compilateur/AST/Noeuds/Operande/NoeudLitteral.h"

INoeud* StrategieLitteral::construire(std::vector<Token>& equation) {
    return new NoeudLitteral(equation[0]);
}
