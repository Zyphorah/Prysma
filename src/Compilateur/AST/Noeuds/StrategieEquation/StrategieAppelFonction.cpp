#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieAppelFonction.h"
#include "Compilateur/AST/ConstructeurArbreEquation.h"
#include "Compilateur/AST/Interfaces/IConstructeurArbre.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Fonction/ParseurAppelFonction.h"
#include "Compilateur/Builder/Equation/ConstructeurEquationFlottante.h"
#include <memory>


StrategieAppelFonction::StrategieAppelFonction(IConstructeurArbre* constructeurEquation) : _constructeurEquation(constructeurEquation)
{}

std::shared_ptr<INoeud> StrategieAppelFonction::construire(std::vector<Token>& equation) {
    ParseurAppelFonction parseurAppel(_constructeurEquation);
    int indexZero = 0;
    std::vector<Token> equationAvecIndex = equation;
    return parseurAppel.parser(equationAvecIndex, indexZero);
}
