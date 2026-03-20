#ifndef PARSEUR_DECLARATIONFONCTION_CPP
#define PARSEUR_DECLARATIONFONCTION_CPP

#include "Compilateur/Fonction/ParseurDeclarationFonction.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Noeuds/NoeudInstruction.h"
#include "Compilateur/AST/Registre/ContextParseur.h"
#include "Compilateur/AST/Registre/Types/IType.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Lexer/TokenType.h"
#include <cstddef>
#include <string>
#include <vector>


ParseurDeclarationFonction::ParseurDeclarationFonction(ContextParseur& contextParseur) 
    : _contextParseur(contextParseur)
{}

ParseurDeclarationFonction::~ParseurDeclarationFonction() = default;

INoeud* ParseurDeclarationFonction::parser(std::vector<Token>& tokens, int& index)
{
  consommer(tokens, index, TOKEN_FONCTION, "Erreur: ce n'est pas le bon token ! 'fn'");

  Token tokenTypeRetour = tokens[static_cast<size_t>(index)];
  IType* typeRetour = _contextParseur.getParseurType()->parser(tokens, index);
  
  Token tokenNomFonction = tokens[static_cast<size_t>(index)];
  std::string nomFonction = tokenNomFonction.value;
  consommer(tokens, index, TOKEN_IDENTIFIANT, "Erreur: identifiant invalide, ce dois être un nom de fonction ");

  // Parser les arguments entre parenthèses
  consommer(tokens, index, TOKEN_PAREN_OUVERTE, "Erreur: ce n'est pas une parenthèse ouverte '('");
  
  std::vector<INoeud*> arguments;
  while(index < static_cast<int>(tokens.size()) && tokens[static_cast<size_t>(index)].type != TOKEN_PAREN_FERMEE)
  {
      if(tokens[static_cast<size_t>(index)].type == TOKEN_VIRGULE)
      {
          index++;
          continue;
      }
      
      INoeud* enfant = _contextParseur.getConstructeurArbreInstruction()->construire(tokens, index);
      arguments.push_back(enfant);
  }

  consommer(tokens, index, TOKEN_PAREN_FERMEE, "Erreur: ce n'est pas une parenthèse fermée ')'");

  // Parser le corps dans un NoeudScope strict
  consommer(tokens, index, TOKEN_ACCOLADE_OUVERTE, "Erreur: ce n'est pas une accolade ouverte '{' ");

  auto* corps = _contextParseur.getConstructeurArbreInstruction()->allouer<NoeudInstruction>();
  consommerEnfantCorps(tokens, index, corps, _contextParseur.getConstructeurArbreInstruction(), TOKEN_ACCOLADE_FERMEE);

  consommer(tokens, index, TOKEN_ACCOLADE_FERMEE, "Erreur: ce n'est pas une accolade fermée '}'");

  auto* noeudFonction = 
      _contextParseur.getConstructeurArbreInstruction()->allouer<NoeudDeclarationFonction>(Token{}, typeRetour, nomFonction, arguments, corps);

  return noeudFonction; 
}

#endif /* PARSEUR_DECLARATIONFONCTION_CPP */




