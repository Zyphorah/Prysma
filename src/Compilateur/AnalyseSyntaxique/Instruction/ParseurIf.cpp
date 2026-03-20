#ifndef PARSEUR_IF_CPP
#define PARSEUR_IF_CPP

#include "Compilateur/Instruction/ParseurIf.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Noeuds/NoeudInstruction.h"
#include "Compilateur/AST/Registre/ContextParseur.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Lexer/TokenType.h"
#include <cstddef>
#include <vector>


ParseurIf::ParseurIf(ContextParseur& contextParseur) 
    : _contextParseur(contextParseur)
{}

ParseurIf::~ParseurIf()
= default;

auto ParseurIf::parser(std::vector<Token>& tokens, int& index) -> INoeud* 
{
  consommer(tokens,index,TOKEN_SI,"Erreur, le token n'est pas 'if'! ");

  consommer(tokens,index,TOKEN_PAREN_OUVERTE,"Erreur, le token n'est pas '('! ");
  

  INoeud* condition = _contextParseur.getConstructeurArbreEquation()->construire(tokens, index);

  consommer(tokens,index,TOKEN_PAREN_FERMEE,"Erreur, le token n'est pas ')'! ");

  // Créer le noeud bloc IF
  auto* noeudBlocIf = _contextParseur.getConstructeurArbreInstruction()->allouer<NoeudInstruction>();
  consommer(tokens,index,TOKEN_ACCOLADE_OUVERTE, "Erreur, le token n'est pas '{'");
  consommerEnfantCorps(tokens,index,noeudBlocIf,_contextParseur.getConstructeurArbreInstruction(),TOKEN_ACCOLADE_FERMEE);
  consommer(tokens,index,TOKEN_ACCOLADE_FERMEE,"Erreur, le token n'est pas '}'");

  // Créer le noeud bloc ELSE s'il existe
  NoeudInstruction* noeudBlocElse = nullptr;
  if (index < static_cast<int>(tokens.size()) && tokens[static_cast<size_t>(index)].type == TOKEN_SINON) {
      consommer(tokens,index,TOKEN_SINON,"Erreur, le token n'est pas 'else'! ");
      noeudBlocElse = _contextParseur.getConstructeurArbreInstruction()->allouer<NoeudInstruction>();
      consommer(tokens,index,TOKEN_ACCOLADE_OUVERTE, "Erreur, le token n'est pas '{'");
      consommerEnfantCorps(tokens,index,noeudBlocElse,_contextParseur.getConstructeurArbreInstruction(),TOKEN_ACCOLADE_FERMEE);
      consommer(tokens,index,TOKEN_ACCOLADE_FERMEE,"Erreur, le token n'est pas '}'");
  }

  // Créer le noeud bloc ENDIF
  auto* noeudBlocEndif = _contextParseur.getConstructeurArbreInstruction()->allouer<NoeudInstruction>();

  auto* noeudIf = _contextParseur.getConstructeurArbreInstruction()->allouer<NoeudIf>(condition, noeudBlocIf, noeudBlocElse, noeudBlocEndif);

  return noeudIf;
}

#endif /* PARSEUR_IF_CPP */




