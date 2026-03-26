#ifndef PARSER_CLASS_CPP
#define PARSER_CLASS_CPP

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
  struct TokenClassName { Token t; };
  struct TokenVisibility { Token t; };

  class ClassParameters {
  public:
    ClassParameters(const TokenClassName& classNameToken, const TokenVisibility& currentVisibility)
        : classNameToken_(classNameToken.t), current_visibility_(currentVisibility.t) {}

    [[nodiscard]] auto classNameToken() const -> const Token& { return classNameToken_; }
    [[nodiscard]] auto current_visibility() const -> const Token& { return current_visibility_; }

  private:
    Token classNameToken_;
    Token current_visibility_;
  };

  void classifyClassNode(INode* node,
              const ClassParameters& param, 
              ContextParser& contextParser,
              std::vector<INode*>& memberList,
              std::vector<INode*>& builders)
  {
    if (node == nullptr) {
      return;
    }

      if (auto* declarationVariable = prysma::dyn_cast<NodeDeclarationVariable>(node)) {
        if (declarationVariable != nullptr) {
          node = contextParser.getBuilderTreeInstruction()->allocate<NodeDeclarationVariable>(
              param.current_visibility(),
              declarationVariable->getNom(),
              declarationVariable->getType(),
              declarationVariable->getExpression()
          );
        }
        memberList.push_back(node);
        return;
      }

      if (auto* declarationFunction = prysma::dyn_cast<NodeDeclarationFunction>(node)) {
        if (declarationFunction != nullptr) {
          node = contextParser.getBuilderTreeInstruction()->allocate<NodeDeclarationFunction>(
              param.current_visibility(),
              declarationFunction->getTypeReturn(),
              declarationFunction->getNom(),
              declarationFunction->getArguments(),
              declarationFunction->getBody()
          );
        }      auto* newDeclarationFunction = prysma::cast<NodeDeclarationFunction>(node);
      if (newDeclarationFunction != nullptr && newDeclarationFunction->getNom() == param.classNameToken().value) {
        builders.push_back(node);
        return;
      }
      memberList.push_back(node);
      return;
    }

    throw CompilationError("Invalid class member: '" + param.classNameToken().value + "'", Line(param.classNameToken().line), Column(param.classNameToken().column));
  }
}


ParserClass::ParserClass(ContextParser& contextParser) 
    : _contextParser(contextParser)
{}

ParserClass::~ParserClass()
= default;

// Example: class ClassName : parent
//          { 
//              private: 
//                  dec int64 privateAttribute;
//                  dec string privateAttribute2;
//                  fn void method(arg int64 param, arg string param2)
//                  {
//                      aff privateAttribute = param;
//                      aff privateAttribute2 = param2;
//                  }
//              public:
//                  fn void ClassName(arg int64 param, arg string param2)
//                  {
//                      aff privateAttribute = param;
//                      aff privateAttribute2 = param2;
//                  }
//                  fn void method(arg int64 param, arg string[] param2)
//                  {
//                      aff privateAttribute = param;
//                      aff privateAttribute2 = param2;
//                  }
//           }

auto ParserClass::parse(std::vector<Token>& tokens, int& index) -> INode*
{
    consume(tokens, index, TOKEN_CLASS, "Expected 'class' at the beginning of the class declaration.");
    Token classNameToken = consume(tokens, index, TOKEN_IDENTIFIER, "Expected an identifier after 'class' for the class name.");
    
    // Inheritance management: class ClassName : Parent
    Token parentToken;
    std::vector<INode*> inheritance; // TODO: handle multiple inheritance 
    
    
    consume(tokens, index, TOKEN_BRACE_OPEN, "Expected '{' after the class name.");
    std::vector<INode*> memberList;
    std::vector<INode*> builders;
    
    // By default, start in PRIVATE section if no visibility keyword
    Token current_visibility;
    current_visibility.type = TOKEN_PRIVATE;
    current_visibility.value = "private";

    while (index < static_cast<int>(tokens.size()) && tokens[static_cast<size_t>(index)].type != TOKEN_BRACE_CLOSE) {
        TokenType tokenType = tokens[static_cast<size_t>(index)].type;

        if (tokenType == TOKEN_PUBLIC) {
            current_visibility = tokens[static_cast<size_t>(index)];
            consume(tokens, index, TOKEN_PUBLIC, "Expected 'public' for the public section of the class.");
            consume(tokens, index, TOKEN_COLON, "Expected ':' after 'public'.");
            continue;
        }

        if (tokenType == TOKEN_PRIVATE) {
            current_visibility = tokens[static_cast<size_t>(index)];
            consume(tokens, index, TOKEN_PRIVATE, "Expected 'private' for the private section of the class.");
            consume(tokens, index, TOKEN_COLON, "Expected ':' after 'private'.");
            continue;
        }

        if (tokenType == TOKEN_PROTECTED) {
          current_visibility = tokens[static_cast<size_t>(index)];
          consume(tokens, index, TOKEN_PROTECTED, "Expected 'protected' for the protected section of the class.");
          consume(tokens, index, TOKEN_COLON, "Expected ':' after 'protected'.");
          continue;
        }

        // If no visibility keyword, parse members in the current section (private by default)
        INode* node = _contextParser.getBuilderTreeInstruction()->build(tokens, index);
        classifyClassNode(node, ClassParameters{TokenClassName{classNameToken}, TokenVisibility{current_visibility}}, _contextParser, memberList, builders);
    }

    consume(tokens, index, TOKEN_BRACE_CLOSE, "Expected '}' at the end of the class declaration.");

    return _contextParser.getBuilderTreeInstruction()->allocate<NodeClass>(inheritance,
                                                                              memberList,
                                                                              builders,
                                                                              classNameToken);

}

#endif /* PARSER_CLASS_CPP */




