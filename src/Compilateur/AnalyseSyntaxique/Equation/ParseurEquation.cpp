#include "Compilateur/AnalyseSyntaxique/Equation/ParseurEquation.h"
#include "Compilateur/Lexer/TokenType.h"

ParseurEquation::ParseurEquation()
{
}

ParseurEquation::~ParseurEquation()
{
}

std::shared_ptr<INoeud> ParseurEquation::parser(std::vector<Token>& tokens, int& index, IConstructeurArbre* constructeurArbre)
{
    auto expression = constructeurArbre->construire(tokens, index);
    consommer(tokens, index, TOKEN_POINT_VIRGULE, "Erreur : ';' attendu à la fin de l'instruction");
    return expression;
}
