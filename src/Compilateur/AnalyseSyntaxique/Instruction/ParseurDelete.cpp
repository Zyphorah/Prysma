#ifndef PARSEUR_DELETE_CPP
#define PARSEUR_DELETE_CPP

#include "Compilateur/Instruction/ParseurDelete.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Registre/ContextParseur.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Lexer/TokenType.h"
#include <vector>


ParseurDelete::ParseurDelete(ContextParseur& contextParseur) 
    : _contextParseur(contextParseur)
{}

ParseurDelete::~ParseurDelete()
= default;

// Exemple : delete variableNom;
auto ParseurDelete::parser(std::vector<Token>& tokens, int& index) -> INoeud*
{
    consommer(tokens, index, TOKEN_DELETE, "Attendu 'delete' au début de l'instruction delete.");
    Token identifiantToken = consommer(tokens, index, TOKEN_IDENTIFIANT, "Attendu un identifiant après 'delete'.");
    consommer(tokens,index, TOKEN_POINT_VIRGULE, "Attendu ';' après l'identifiant dans l'instruction delete.");

    return _contextParseur.getConstructeurArbreInstruction()->allouer<NoeudDelete>(identifiantToken);
}

#endif /* PARSEUR_DELETE_CPP */




