#ifndef PARSEUR_RETOUR_CPP
#define PARSEUR_RETOUR_CPP

#include "Compilateur/Fonction/ParseurRetour.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Registre/ContextParseur.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Lexer/TokenType.h"
#include <cstddef>
#include <vector>


ParseurRetour::ParseurRetour(ContextParseur& contextParseur) 
    : _contextParseur(contextParseur)
{}

ParseurRetour::~ParseurRetour() = default;

auto ParseurRetour::parser(std::vector<Token>& tokens, int& index) -> INoeud*
{
  consommer(tokens, index, TOKEN_RETOUR, "Erreur: ce n'est pas le bon token ! 'return'");

  INoeud* valeurRetour = nullptr;

  if (index < static_cast<int>(tokens.size()) && tokens[static_cast<size_t>(index)].type != TOKEN_POINT_VIRGULE) {
      valeurRetour = _contextParseur.getConstructeurArbreEquation()->construire(tokens, index);
  } else {
      consommer(tokens, index, TOKEN_POINT_VIRGULE, "Erreur: point-virgule attendu après return");
      return _contextParseur.getConstructeurArbreEquation()->allouer<NoeudRetour>(valeurRetour);
  }
  
  consommer(tokens, index, TOKEN_POINT_VIRGULE, "Erreur : ';' attendu à la fin du return");

  return _contextParseur.getConstructeurArbreEquation()->allouer<NoeudRetour>(valeurRetour);
}

#endif /* PARSEUR_RETOUR_CPP */




