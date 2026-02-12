#include "Compilateur/AnalyseSyntaxique/Equation/ParseurEquation.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/Builder/Equation/ConstructeurEquationFlottante.h"

ParseurEquation::ParseurEquation()
{
}

ParseurEquation::~ParseurEquation()
{
}

std::shared_ptr<INoeud> ParseurEquation::parser(std::vector<Token>& tokens, int& index, IConstructeurArbre* constructeurArbre)
{
    ConstructeurEquationFlottante constructeurEquation(constructeurArbre);
    auto expression = constructeurEquation.recupererConstructeurArbre()->construire(tokens, index);

    if(tokens[index].type != TOKEN_CALL) // skip en cas d'instruction car il contient un parseur qui mange déjà les tokens ce qui provoque un crash
    {
        consommer(tokens, index, TOKEN_POINT_VIRGULE, "Erreur : ';' attendu à la fin de l'instruction");
    }

    return expression;
}
