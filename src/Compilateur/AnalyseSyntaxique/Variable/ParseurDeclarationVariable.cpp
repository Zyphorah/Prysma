#ifndef PARSEUR_DECLARATIONVARIABLE_CPP
#define PARSEUR_DECLARATIONVARIABLE_CPP

#include "Compilateur/Variable/ParseurDeclarationVariable.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Registre/ContextParseur.h"
#include "Compilateur/AST/Registre/Types/IType.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Lexer/TokenType.h"
#include <string>
#include <vector>


ParseurDeclarationVariable::ParseurDeclarationVariable(ContextParseur& contextParseur) 
    : _contextParseur(contextParseur)
{}

ParseurDeclarationVariable::~ParseurDeclarationVariable()
= default;

auto ParseurDeclarationVariable::parser(std::vector<Token>& tokens, int& index) -> INoeud*
{
    consommer(tokens, index, TOKEN_DEC, "Erreur : type attendu 'dec");
    
    // Utiliser le ParseurType pour analyser le type (simple ou tableau)
    IType* type = _contextParseur.getParseurType()->parser(tokens, index);
    
    Token nomToken = consommer(tokens, index, TOKEN_IDENTIFIANT, "Erreur : nom de variable attendu");
    std::string nomVariable = nomToken.value;
    
    consommer(tokens, index, TOKEN_EGAL, "Erreur : '=' attendu après le nom de variable");
    
    INoeud* expression = _contextParseur.getConstructeurArbreEquation()->construire(tokens, index);
    
    consommer(tokens, index, TOKEN_POINT_VIRGULE, "Erreur : ';' attendu à la fin de la déclaration");

    return _contextParseur.getConstructeurArbreEquation()->allouer<NoeudDeclarationVariable>(
        Token{},
        nomVariable,
        type,
        expression
    );
}
#endif /* PARSEUR_DECLARATIONVARIABLE_CPP */




