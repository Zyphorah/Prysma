#include "Compilateur/GestionnaireErreur.h"
#include <cstddef>
#ifndef PARSEUR_CLASS_CPP
#define PARSEUR_CLASS_CPP

#include "Compilateur/objet/ParseurClass.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Registre/ContextParseur.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/Visiteur/Interfaces/IVisiteur.h"
#include "Compilateur/Utils/PrysmaCast.h"
#include <vector>

namespace
{
  void classerNoeudClasse(INoeud* noeud,
              const Token& nomClasseToken, // NOLINT(bugprone-easily-swappable-parameters)
              const Token& visibilite_courante, // NOLINT(bugprone-easily-swappable-parameters)
              ContextParseur& contextParseur,
              std::vector<INoeud*>& listMembres,
              std::vector<INoeud*>& constructeurs)
  {
    if (noeud == nullptr) {
      return;
    }

    if (auto* declarationVariable = prysma::dyn_cast<NoeudDeclarationVariable>(noeud)) {
      if (declarationVariable != nullptr) {
        noeud = contextParseur.getConstructeurArbreInstruction()->allouer<NoeudDeclarationVariable>(
            visibilite_courante,
            declarationVariable->getNom(),
            declarationVariable->getType(),
            declarationVariable->getExpression()
        );
      }
      listMembres.push_back(noeud);
      return;
    }

    if (auto* declarationFonction = prysma::dyn_cast<NoeudDeclarationFonction>(noeud)) {
      if (declarationFonction != nullptr) {
        noeud = contextParseur.getConstructeurArbreInstruction()->allouer<NoeudDeclarationFonction>(
            visibilite_courante,
            declarationFonction->getTypeRetour(),
            declarationFonction->getNom(),
            declarationFonction->getArguments(),
            declarationFonction->getCorps()
        );
      }
      
      auto* newDeclarationFonction = prysma::cast<NoeudDeclarationFonction>(noeud);
      if (newDeclarationFonction != nullptr && newDeclarationFonction->getNom() == nomClasseToken.value) {
        constructeurs.push_back(noeud);
        return;
      }
      listMembres.push_back(noeud);
      return;
    }

    throw ErreurCompilation("Membre de classe invalide : '" + nomClasseToken.value + "'", Ligne(nomClasseToken.ligne), Colonne(nomClasseToken.colonne));
  }
}


ParseurClass::ParseurClass(ContextParseur& contextParseur) 
    : _contextParseur(contextParseur)
{}

ParseurClass::~ParseurClass()
= default;

// Exemple: class NomClasse : parent
//          { 
//              private: 
//                  dec int64 attributPrive;
//                  dec string attributPrive2;
//                  fn void methode(arg int64 parametre, arg string parametre2)
//                  {
//                      aff attributPrive = parametre;
//                      aff attributPrive2 = parametre2;
//                  }
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
//           }

INoeud* ParseurClass::parser(std::vector<Token>& tokens, int& index)
{
    consommer(tokens, index, TOKEN_CLASS, "Attendu 'class' au début de la déclaration de classe.");
    Token nomClasseToken = consommer(tokens, index, TOKEN_IDENTIFIANT, "Attendu un identifiant après 'class' pour le nom de la classe.");
    
    // Gestion de l'héritage : class NomClasse : Parent
    Token parentToken;
    std::vector<INoeud*> heritage; // TODO : gérer plusieurs héritages 
    
    
    consommer(tokens, index, TOKEN_ACCOLADE_OUVERTE, "Attendu '{' après le nom de la classe.");
    std::vector<INoeud*> listMembres;
    std::vector<INoeud*> constructeurs;
    
    // Par défaut, on commence en section PRIVATE si pas de mot-clé de visibilité
    Token visibilite_courante;
    visibilite_courante.type = TOKEN_PRIVATE;
    visibilite_courante.value = "private";

    while (index < static_cast<int>(tokens.size()) && tokens[static_cast<size_t>(index)].type != TOKEN_ACCOLADE_FERMEE) {
        TokenType tokenType = tokens[static_cast<size_t>(index)].type;

        if (tokenType == TOKEN_PUBLIC) {
            visibilite_courante = tokens[static_cast<size_t>(index)];
            consommer(tokens, index, TOKEN_PUBLIC, "Attendu 'public' pour la section publique de la classe.");
            consommer(tokens, index, TOKEN_DEUX_POINTS, "Attendu ':' après 'public'.");
            continue;
        }

        if (tokenType == TOKEN_PRIVATE) {
            visibilite_courante = tokens[static_cast<size_t>(index)];
            consommer(tokens, index, TOKEN_PRIVATE, "Attendu 'private' pour la section privée de la classe.");
            consommer(tokens, index, TOKEN_DEUX_POINTS, "Attendu ':' après 'private'.");
            continue;
        }

        if (tokenType == TOKEN_PROTECTED) {
          visibilite_courante = tokens[static_cast<size_t>(index)];
          consommer(tokens, index, TOKEN_PROTECTED, "Attendu 'protected' pour la section protégée de la classe.");
          consommer(tokens, index, TOKEN_DEUX_POINTS, "Attendu ':' après 'protected'.");
          continue;
        }

        // Si aucun mot-clé de visibilité, on parse les membres dans la section courante (private par défaut)
        INoeud* noeud = _contextParseur.getConstructeurArbreInstruction()->construire(tokens, index);
        classerNoeudClasse(noeud, nomClasseToken, visibilite_courante, _contextParseur, listMembres, constructeurs);
    }

    consommer(tokens, index, TOKEN_ACCOLADE_FERMEE, "Attendu '}' à la fin de la déclaration de classe.");

    return _contextParseur.getConstructeurArbreInstruction()->allouer<NoeudClass>(heritage,
                                                                              listMembres,
                                                                              constructeurs,
                                                                              nomClasseToken);

}

#endif /* PARSEUR_CLASS_CPP */




