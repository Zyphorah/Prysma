#ifndef PARSEUR_INCLUDE_CPP
#define PARSEUR_INCLUDE_CPP

#include "Compilateur/Instruction/ParseurInclude.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Registre/ContextParseur.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Lexer/TokenType.h"
#include <vector>


ParseurInclude::ParseurInclude(ContextParseur& contextParseur) 
    : _contextParseur(contextParseur)
{}

ParseurInclude::~ParseurInclude()
= default;

// Exemple : include "path"
auto ParseurInclude::parser(std::vector<Token>& tokens, int& index) -> INoeud*
{
    consommer(tokens,index,TOKEN_INCLUDE, "Erreur : Instruction include doit commencer par le mot-clé 'include'");
    consommer(tokens,index,TOKEN_GUILLEMET, "Erreur : Instruction include doit être suivie d'une chaîne de caractères entre guillemets");
    Token tokenPath = consommer(tokens,index,TOKEN_IDENTIFIANT, "Erreur : Instruction include doit contenir un chemin de fichier");
    consommer(tokens,index,TOKEN_GUILLEMET, "Erreur : Instruction include doit être suivie d'une chaîne de caractères entre guillemets");
    consommer(tokens,index,TOKEN_POINT_VIRGULE, "Erreur : Instruction include doit se terminer par un point-virgule");

    return _contextParseur.getConstructeurArbreEquation()->allouer<NoeudInclude>(tokenPath.value);
}

#endif /* PARSEUR_INCLUDE_CPP */




