#ifndef PARSEUR_CLASS_CPP
#define PARSEUR_CLASS_CPP

#include "Compilateur/objet/ParseurClass.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/Visiteur/Interfaces/IVisiteur.h"


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
    
    // Déterminer si c'est un public ou un private 
    TokenType tokenType = tokens[static_cast<size_t>(index)].type;
    while (tokenType == TOKEN_PUBLIC || tokenType == TOKEN_PRIVATE) {

      if(tokenType == TOKEN_PUBLIC) {
        consommer(tokens, index, TOKEN_PUBLIC, "Attendu 'public' pour la section publique de la classe.");
       // consommerEnfantCorps(tokens, index,);

      } else {
        consommer(tokens, index, TOKEN_PRIVATE, "Attendu 'private' pour la section privée de la classe.");
      }
    
    }

    //return _contextParseur.constructeurArbreInstruction->allouer<NoeudClass>(nomClasseToken);
    return nullptr;

}

#endif /* PARSEUR_CLASS_CPP */




