#include "Compilateur/AnalyseSyntaxique/Instruction/Variable/ParseurDeclarationVariable.h"
#include "Compilateur/AST/Noeuds/Variable/NoeudDeclarationVariable.h"
#include "Compilateur/Lexer/TokenType.h"
#include <memory>
#include <utility>

ParseurDeclarationVariable::ParseurDeclarationVariable(ParseurType* parseurType, IConstructeurArbre* constructeurEquation)
    : _parseurType(parseurType), _constructeurEquation(constructeurEquation)
{}

ParseurDeclarationVariable::~ParseurDeclarationVariable()
{
}

INoeud* ParseurDeclarationVariable::parser(std::vector<Token>& tokens, int& index)
{
    consommer(tokens, index, TOKEN_DEC, "Erreur : type attendu 'dec");
    
    // Utiliser le ParseurType pour analyser le type (simple ou tableau)
    IType* type = _parseurType->parser(tokens, index);
    
    Token nomToken = consommer(tokens, index, TOKEN_IDENTIFIANT, "Erreur : nom de variable attendu");
    std::string nomVariable = nomToken.value;
    
    consommer(tokens, index, TOKEN_EGAL, "Erreur : '=' attendu après le nom de variable");
    
    INoeud* expression = _constructeurEquation->construire(tokens, index);
    
    consommer(tokens, index, TOKEN_POINT_VIRGULE, "Erreur : ';' attendu à la fin de la déclaration");

    return new NoeudDeclarationVariable(
        nomVariable,
        type,
        expression
    );
}
