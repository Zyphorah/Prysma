#include "Compilateur/AnalyseSyntaxique/Equation/ParseurEquation.h"
#include "Compilateur/AST/Interfaces/IConstructeurArbre.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/Builder/Equation/ConstructeurEquationFlottante.h"

ParseurEquation::ParseurEquation(IConstructeurArbre* constructeurArbre) : _constructeurArbre(constructeurArbre)
{
}

ParseurEquation::~ParseurEquation()
{
}

INoeud* ParseurEquation::parser(std::vector<Token>& tokens, int& index)
{

    INoeud* expression = _constructeurArbre->construire(tokens, index);

    consommer(tokens, index, TOKEN_POINT_VIRGULE, "Erreur : ';' attendu à la fin de l'instruction");
    
    return expression;
}
