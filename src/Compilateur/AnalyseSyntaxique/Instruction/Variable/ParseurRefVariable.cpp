#include "Compilateur/AnalyseSyntaxique/ParseurRefVariable.h"
#include "Compilateur/AST/Registre/ContextParseur.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include <vector>

ParseurRefVariable::ParseurRefVariable(ContextParseur& contextParseur)
    : _contextParseur(contextParseur)
{
}

ParseurRefVariable::~ParseurRefVariable()
{
}

// Exemple ref variable
INoeud* ParseurRefVariable::parser(std::vector<Token>& tokens, int& index) 
{
    consommer(tokens, index, TOKEN_REF, "Erreur : 'ref' attendu");
    
    Token nomToken = consommer(tokens, index, TOKEN_IDENTIFIANT, "Erreur : nom de variable attendu après 'ref'");
    std::string nomVariable = nomToken.value;
    
    return _contextParseur.constructeurArbreEquation->allouer<NoeudRefVariable>(nomVariable);
}