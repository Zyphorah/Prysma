#ifndef PARSEUR_ARGFONCTION_CPP
#define PARSEUR_ARGFONCTION_CPP

#include "Compilateur/Fonction/ParseurArgFonction.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Registre/ContextParseur.h"
#include "Compilateur/AST/Registre/Types/IType.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Lexer/TokenType.h"
#include <vector>


ParseurArgFonction::ParseurArgFonction(ContextParseur& contextParseur) 
    : _contextParseur(contextParseur)
{}

ParseurArgFonction::~ParseurArgFonction()
= default;

auto ParseurArgFonction::parser(std::vector<Token>& tokens, int& index) -> INoeud* 
{

  consommer(tokens,index,TOKEN_ARG,"Erreur: le token n'est pas 'arg' !");

  IType* type = _contextParseur.getParseurType()->parser(tokens, index);

  Token nom = consommer(tokens,index,TOKEN_IDENTIFIANT,"Erreur: ce n'est pas un identifiant!");

  return _contextParseur.getConstructeurArbreEquation()->allouer<NoeudArgFonction>(type, nom.value);
}

#endif /* PARSEUR_ARGFONCTION_CPP */




