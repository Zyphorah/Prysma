#include "Compilateur/AnalyseSyntaxique/Equation/ParseurEquation.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/Builder/Equation/ConstructeurEquationFlottante.h"
#include <memory>

ParseurEquation::ParseurEquation()
{
}

ParseurEquation::~ParseurEquation()
{
}

std::shared_ptr<INoeud> ParseurEquation::parser(std::vector<Token>& tokens, int& index, IConstructeurArbre* constructeurArbre)
{
    ConstructeurEquationFlottante constructeurEquation(constructeurArbre);
    std::shared_ptr<INoeud> expression = constructeurEquation.recupererConstructeurArbre()->construire(tokens, index);

    consommer(tokens, index, TOKEN_POINT_VIRGULE, "Erreur : ';' attendu à la fin de l'instruction");
    
    return expression;
}
