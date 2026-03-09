#include "Compilateur/AnalyseSyntaxique/ParseurDeclarationVariable.h"
#include "Compilateur/AST/AST_Genere.h"


ParseurDeclarationVariable::ParseurDeclarationVariable(ContextParseur& contextParseur)
    : _contextParseur(contextParseur)
{}

ParseurDeclarationVariable::~ParseurDeclarationVariable()
{
}

INoeud* ParseurDeclarationVariable::parser(std::vector<Token>& tokens, int& index)
{
    consommer(tokens, index, TOKEN_DEC, "Erreur : type attendu 'dec");
    
    // Utiliser le ParseurType pour analyser le type (simple ou tableau)
    IType* type = _contextParseur.parseurType->parser(tokens, index);
    
    Token nomToken = consommer(tokens, index, TOKEN_IDENTIFIANT, "Erreur : nom de variable attendu");
    std::string nomVariable = nomToken.value;
    
    consommer(tokens, index, TOKEN_EGAL, "Erreur : '=' attendu après le nom de variable");
    
    INoeud* expression = _contextParseur.constructeurArbreEquation->construire(tokens, index);
    
    consommer(tokens, index, TOKEN_POINT_VIRGULE, "Erreur : ';' attendu à la fin de la déclaration");

    return _contextParseur.constructeurArbreEquation->allouer<NoeudDeclarationVariable>(
        nomVariable,
        type,
        expression
    );
}
