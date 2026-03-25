#ifndef PARSEUR_CLASS_CPP
#define PARSEUR_CLASS_CPP

#include "Compiler/ManagerError.h"
#include <cstddef>
#include "Compiler/Object/ParserClass.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/ContextParser.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include "Compiler/Visitor/Interfaces/IVisitor.h"
#include "Compiler/Utils/PrysmaCast.h"
#include <vector>

namespace
{
  struct TokenNomClasse { Token t; };
  struct TokenVisibilite { Token t; };

  class ParametresClasse {
  public:
    ParametresClasse(const TokenNomClasse& nomClasseToken, const TokenVisibilite& visibiliteCourante)
        : nomClasseToken_(nomClasseToken.t), visibilite_courante_(visibiliteCourante.t) {}

    [[nodiscard]] auto nomClasseToken() const -> const Token& { return nomClasseToken_; }
    [[nodiscard]] auto visibilite_courante() const -> const Token& { return visibilite_courante_; }

  private:
    Token nomClasseToken_;
    Token visibilite_courante_;
  };

  void classerNodeClasse(INode* node,
              const ParametresClasse& param, 
              ContextParser& contextParser,
              std::vector<INode*>& listMembers,
              std::vector<INode*>& builders)
  {
    if (node == nullptr) {
      return;
    }

      if (auto* declarationVariable = prysma::dyn_cast<NodeDeclarationVariable>(node)) {
        if (declarationVariable != nullptr) {
          node = contextParser.getBuilderTreeInstruction()->allouer<NodeDeclarationVariable>(
              param.visibilite_courante(),
              declarationVariable->getNom(),
              declarationVariable->getType(),
              declarationVariable->getExpression()
          );
        }
        listMembers.push_back(node);
        return;
      }

      if (auto* declarationFunction = prysma::dyn_cast<NodeDeclarationFunction>(node)) {
        if (declarationFunction != nullptr) {
          node = contextParser.getBuilderTreeInstruction()->allouer<NodeDeclarationFunction>(
              param.visibilite_courante(),
              declarationFunction->getTypeReturn(),
              declarationFunction->getNom(),
              declarationFunction->getArguments(),
              declarationFunction->getBody()
          );
        }      auto* newDeclarationFunction = prysma::cast<NodeDeclarationFunction>(node);
      if (newDeclarationFunction != nullptr && newDeclarationFunction->getNom() == param.nomClasseToken().value) {
        builders.push_back(node);
        return;
      }
      listMembers.push_back(node);
      return;
    }

    throw ErrorCompilation("Membre de classe invalide : '" + param.nomClasseToken().value + "'", Ligne(param.nomClasseToken().ligne), Colonne(param.nomClasseToken().colonne));
  }
}


ParserClass::ParserClass(ContextParser& contextParser) 
    : _contextParser(contextParser)
{}

ParserClass::~ParserClass()
= default;

// Exemple: class NomClasse : parent
//          { 
//              private: 
//                  dec int64 attributePrive;
//                  dec string attributePrive2;
//                  fn void methode(arg int64 parametre, arg string parametre2)
//                  {
//                      aff attributePrive = parametre;
//                      aff attributePrive2 = parametre2;
//                  }
//              public:
//                  fn void NomClasse(arg int64 parametre, arg string parametre2)
//                  {
//                      aff attributePrive = parametre;
//                      aff attributePrive2 = parametre2;
//                  }
//                  fn void methode(arg int64 parametre, arg string[] parametre2)
//                  {
//                      aff attributePrive = parametre;
//                      aff attributePrive2 = parametre2;
//                  }
//           }

auto ParserClass::parser(std::vector<Token>& tokens, int& index) -> INode*
{
    consommer(tokens, index, TOKEN_CLASS, "Attendu 'class' au début de la déclaration de classe.");
    Token nomClasseToken = consommer(tokens, index, TOKEN_IDENTIFIANT, "Attendu un identifiant après 'class' pour le nom de la classe.");
    
    // Gestion de l'héritage : class NomClasse : Parent
    Token parentToken;
    std::vector<INode*> heritage; // TODO : gérer plusieurs héritages 
    
    
    consommer(tokens, index, TOKEN_ACCOLADE_OUVERTE, "Attendu '{' après le nom de la classe.");
    std::vector<INode*> listMembers;
    std::vector<INode*> builders;
    
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
        INode* node = _contextParser.getBuilderTreeInstruction()->construire(tokens, index);
        classerNodeClasse(node, ParametresClasse{TokenNomClasse{nomClasseToken}, TokenVisibilite{visibilite_courante}}, _contextParser, listMembers, builders);
    }

    consommer(tokens, index, TOKEN_ACCOLADE_FERMEE, "Attendu '}' à la fin de la déclaration de classe.");

    return _contextParser.getBuilderTreeInstruction()->allouer<NodeClass>(heritage,
                                                                              listMembers,
                                                                              builders,
                                                                              nomClasseToken);

}

#endif /* PARSEUR_CLASS_CPP */




