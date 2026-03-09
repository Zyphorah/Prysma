#include "Compilateur/AnalyseSyntaxique/ParseurUnRefVariable.h"
#include "Compilateur/AST/AST_Genere.h"

ParseurUnRefVariable::ParseurUnRefVariable(ContextParseur& contextParseur)
    : _contextParseur(contextParseur)
{
}

ParseurUnRefVariable::~ParseurUnRefVariable()
{
}

// Exemple unref variable
INoeud* ParseurUnRefVariable::parser(std::vector<Token>& tokens, int& index) 
{
    consommer(tokens, index, TOKEN_UNREF, "Erreur : 'unref' attendu");
    
    Token nomToken = consommer(tokens, index, TOKEN_IDENTIFIANT, "Erreur : nom de variable attendu après 'unref'");
    std::string nomVariable = nomToken.value;
    
    return _contextParseur.constructeurArbreEquation->allouer<NoeudUnRefVariable>(nomVariable);
}