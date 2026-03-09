#include "Compilateur/AST/Noeuds/StrategieEquation/StrategieAppelFonction.h"
#include "Compilateur/AST/Registre/ContextParseur.h"
#include "Compilateur/AnalyseSyntaxique/ParseurAppelFonction.h"
#include "Compilateur/Builder/Equation/ConstructeurEquationFlottante.h"


StrategieAppelFonction::StrategieAppelFonction(ContextParseur& contextParseur) : _contextParseur(contextParseur)
{}

INoeud* StrategieAppelFonction::construire(std::vector<Token>& equation) {
    ParseurAppelFonction parseurAppel(_contextParseur);
    int indexZero = 0;
    return parseurAppel.parser(equation, indexZero);
}