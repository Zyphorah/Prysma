#include "Compilateur/AnalyseSyntaxique/ParseurEquation.h"
#include "Compilateur/AST/Registre/ContextParseur.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/Builder/Equation/ConstructeurEquationFlottante.h"

ParseurEquation::ParseurEquation(ContextParseur& contextParseur) : _contextParseur(contextParseur)
{
}

ParseurEquation::~ParseurEquation()
{
}

INoeud* ParseurEquation::parser(std::vector<Token>& tokens, int& index)
{

    INoeud* expression = _contextParseur.constructeurArbreEquation->construire(tokens, index);

    consommer(tokens, index, TOKEN_POINT_VIRGULE, "Erreur : ';' attendu à la fin de l'instruction");
    
    return expression;
}
