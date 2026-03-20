#ifndef PARSEUR_REFVARIABLE_CPP
#define PARSEUR_REFVARIABLE_CPP

#include "Compilateur/Variable/ParseurRefVariable.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Registre/ContextParseur.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Lexer/TokenType.h"
#include <string>
#include <vector>


ParseurRefVariable::ParseurRefVariable(ContextParseur& contextParseur) 
    : _contextParseur(contextParseur)
{}

ParseurRefVariable::~ParseurRefVariable()
= default;

// Exemple ref variable
auto ParseurRefVariable::parser(std::vector<Token>& tokens, int& index) -> INoeud* 
{
    consommer(tokens, index, TOKEN_REF, "Erreur : 'ref' attendu");
    
    Token nomToken = consommer(tokens, index, TOKEN_IDENTIFIANT, "Erreur : nom de variable attendu après 'ref'");
    std::string nomVariable = nomToken.value;
    
    return _contextParseur.getConstructeurArbreEquation()->allouer<NoeudRefVariable>(nomVariable);
}

#endif /* PARSEUR_REFVARIABLE_CPP */




