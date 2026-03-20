#ifndef PARSEUR_APPELFONCTION_CPP
#define PARSEUR_APPELFONCTION_CPP

#include "Compilateur/Fonction/ParseurAppelFonction.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/AST/Noeuds/Interfaces/IInstruction.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Registre/ContextParseur.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Lexer/TokenType.h"
#include <cstddef>
#include <vector>


ParseurAppelFonction::ParseurAppelFonction(ContextParseur& contextParseur) 
    : _contextParseur(contextParseur)
{}

ParseurAppelFonction::~ParseurAppelFonction()
= default;

INoeud* ParseurAppelFonction::parser(std::vector<Token>& tokens, int& index)
{
  const bool appelCommeInstruction = index == 0 || tokens[static_cast<size_t>(index - 1)].type != TOKEN_EGAL;

  consommer(tokens, index, TOKEN_CALL, "Erreur: 'call' attendu");
  Token nomFonction = consommer(tokens, index, TOKEN_IDENTIFIANT, "Erreur: identifiant de fonction attendu");
  consommer(tokens, index, TOKEN_PAREN_OUVERTE, "Erreur: '(' attendue");
  
  IInstruction* noeudAppel = _contextParseur.getConstructeurArbreEquation()->allouer<NoeudAppelFonction>(nomFonction);
  
  consommerEnfantCorps(tokens, index, noeudAppel, _contextParseur.getConstructeurArbreEquation(), TOKEN_PAREN_FERMEE);

  consommer(tokens, index, TOKEN_PAREN_FERMEE, "Erreur: ')' attendue");

  if (appelCommeInstruction && index < static_cast<int>(tokens.size()) && tokens[static_cast<size_t>(index)].type == TOKEN_POINT_VIRGULE) {
      consommer(tokens, index, TOKEN_POINT_VIRGULE, "Erreur : ';' attendu à la fin de l'appel de fonction");
  }

  return noeudAppel;
}

#endif /* PARSEUR_APPELFONCTION_CPP */




