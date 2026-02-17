#include "Compilateur/AnalyseSyntaxique/Equation/ParseurEquation.h"
#include "Compilateur/AST/Interfaces/IConstructeurArbre.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/Builder/Equation/ConstructeurEquationFlottante.h"
#include <memory>

ParseurEquation::ParseurEquation(IConstructeurArbre* constructeurArbre) : _constructeurArbre(constructeurArbre)
{
}

ParseurEquation::~ParseurEquation()
{
}

std::shared_ptr<INoeud> ParseurEquation::parser(std::vector<Token>& tokens, int& index, IConstructeurArbre* constructeurArbre)
{
    // Use the stored equation builder, not the instruction builder parameter
    std::shared_ptr<INoeud> expression = _constructeurArbre->construire(tokens, index);

    consommer(tokens, index, TOKEN_POINT_VIRGULE, "Erreur : ';' attendu à la fin de l'instruction");
    
    return expression;
}
