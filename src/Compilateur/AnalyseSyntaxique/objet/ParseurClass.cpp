#ifndef PARSEUR_CLASS_CPP
#define PARSEUR_CLASS_CPP

#include "Compilateur/objet/ParseurClass.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/GestionnaireErreur.h"
#include "Compilateur/Visiteur/Interfaces/IVisiteur.h"

namespace
{
  Token consommerSectionClasse(std::vector<Token>& tokens, int& index, TokenType typeAttendu, const std::string& messageErreur)
  {
    if (index < 0 || index >= static_cast<int>(tokens.size()) || tokens[static_cast<size_t>(index)].type != typeAttendu) {
      if (index >= 0 && index < static_cast<int>(tokens.size())) {
        throw ErreurCompilation(messageErreur, tokens[static_cast<size_t>(index)].ligne, tokens[static_cast<size_t>(index)].colonne);
      }
      throw ErreurCompilation(messageErreur, 1, 1);
    }
    return tokens[static_cast<size_t>(index++)];
  }

  void classerNoeudClasse(INoeud* noeud,
              const Token& nomClasseToken,
              std::vector<INoeud*>& variables,
              std::vector<INoeud*>& fonctions,
              std::vector<INoeud*>& constructeurs)
  {
    if (noeud == nullptr) {
      return;
    }

    if (noeud->getTypeGenere() == NoeudTypeGenere::DeclarationVariable) {
      variables.push_back(noeud);
      return;
    }

    if (noeud->getTypeGenere() == NoeudTypeGenere::DeclarationFonction) {
      auto* declarationFonction = dynamic_cast<NoeudDeclarationFonction*>(noeud);
      if (declarationFonction != nullptr && declarationFonction->getNom() == nomClasseToken.value) {
        constructeurs.push_back(noeud);
        return;
      }
      fonctions.push_back(noeud);
      return;
    }

    throw ErreurCompilation("Membre de classe invalide : '" + nomClasseToken.value + "'", nomClasseToken.ligne, nomClasseToken.colonne);
  }

  void parserSectionClasse(std::vector<Token>& tokens,
               int& index,
               const Token& nomClasseToken,
               ContextParseur& contextParseur,
               std::vector<INoeud*>& variables,
               std::vector<INoeud*>& fonctions,
               std::vector<INoeud*>& constructeurs)
  {
    while (index < static_cast<int>(tokens.size())
      && tokens[static_cast<size_t>(index)].type != TOKEN_FIN
      && tokens[static_cast<size_t>(index)].type != TOKEN_PUBLIC
      && tokens[static_cast<size_t>(index)].type != TOKEN_PRIVATE
      && tokens[static_cast<size_t>(index)].type != TOKEN_ACCOLADE_FERMEE) {
      INoeud* noeud = contextParseur.constructeurArbreInstruction->construire(tokens, index);
      classerNoeudClasse(noeud, nomClasseToken, variables, fonctions, constructeurs);
    }

    consommerSectionClasse(tokens, index, TOKEN_FIN, "Attendu 'fin' pour terminer la section de classe.");
    consommerSectionClasse(tokens, index, TOKEN_POINT_VIRGULE, "Attendu ';' après 'fin' dans la section de classe.");
  }
}


ParseurClass::ParseurClass(ContextParseur& contextParseur) 
    : _contextParseur(contextParseur)
{}

ParseurClass::~ParseurClass()
{}

// Exemple: class NomClasse 
//          { 
//              private: 
//                  dec int64 attributPrive;
//                  dec string attributPrive2;
//                  fn void methode(arg int64 parametre, arg string parametre2)
//                  {
//                      aff attributPrive = parametre;
//                      aff attributPrive2 = parametre2;
//                  }
//              fin;
//              public:
//                  fn void NomClasse(arg int64 parametre, arg string parametre2)
//                  {
//                      aff attributPrive = parametre;
//                      aff attributPrive2 = parametre2;
//                  }
//                  fn void methode(arg int64 parametre, arg string[] parametre2)
//                  {
//                      aff attributPrive = parametre;
//                      aff attributPrive2 = parametre2;
//                  }
//              fin;
//           }

INoeud* ParseurClass::parser(std::vector<Token>& tokens, int& index)
{
    consommer(tokens, index, TOKEN_CLASS, "Attendu 'class' au début de la déclaration de classe.");
    Token nomClasseToken = consommer(tokens, index, TOKEN_IDENTIFIANT, "Attendu un identifiant après 'class' pour le nom de la classe.");
    
    consommer(tokens, index, TOKEN_ACCOLADE_OUVERTE, "Attendu '{' après le nom de la classe.");

    std::vector<INoeud*> heritage;
    std::vector<INoeud*> listVariable;
    std::vector<INoeud*> listFonction;
    std::vector<INoeud*> listVariablePrive;
    std::vector<INoeud*> listFonctionPrive;
    std::vector<INoeud*> listVariableProtected;
    std::vector<INoeud*> listFonctionProtected;
    std::vector<INoeud*> constructeurs;

    while (index < static_cast<int>(tokens.size()) && tokens[static_cast<size_t>(index)].type != TOKEN_ACCOLADE_FERMEE) {
        TokenType tokenType = tokens[static_cast<size_t>(index)].type;

        if (tokenType == TOKEN_PUBLIC) {
            consommer(tokens, index, TOKEN_PUBLIC, "Attendu 'public' pour la section publique de la classe.");
            parserSectionClasse(tokens, index, nomClasseToken, _contextParseur, listVariable, listFonction, constructeurs);
            continue;
        }

        if (tokenType == TOKEN_PRIVATE) {
            consommer(tokens, index, TOKEN_PRIVATE, "Attendu 'private' pour la section privée de la classe.");
            parserSectionClasse(tokens, index, nomClasseToken, _contextParseur, listVariablePrive, listFonctionPrive, constructeurs);
            continue;
        }

        if (tokenType == TOKEN_PROTECTED) {
          consommer(tokens, index, TOKEN_PROTECTED, "Attendu 'protected' pour la section protégée de la classe.");
          parserSectionClasse(tokens, index, nomClasseToken, _contextParseur, listVariableProtected, listFonctionProtected, constructeurs);
          continue;
        }

        throw ErreurCompilation("Section de classe invalide.", tokens[static_cast<size_t>(index)].ligne, tokens[static_cast<size_t>(index)].colonne);
    }

    consommer(tokens, index, TOKEN_ACCOLADE_FERMEE, "Attendu '}' à la fin de la déclaration de classe.");

    return _contextParseur.constructeurArbreInstruction->allouer<NoeudClass>(heritage,
                                                                              listVariable,
                                                                              listFonction,
                                                                              listVariablePrive,
                                                                              listFonctionPrive,
                                                                              listVariableProtected,
                                                                              listFonctionProtected,
                                                                              constructeurs,
                                                                              nomClasseToken);

}

#endif /* PARSEUR_CLASS_CPP */




