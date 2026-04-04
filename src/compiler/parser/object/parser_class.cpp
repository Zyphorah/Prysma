//===-- parser_class.cpp -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#ifndef PARSER_CLASS_CPP
#define PARSER_CLASS_CPP

#include "compiler/manager_error.h"
#include <cstddef>
#include "compiler/object/parser_class.h"
#include "compiler/ast/ast_genere.h"
#include "compiler/ast/nodes/interfaces/i_node.h"
#include "compiler/ast/registry/context_parser.h"
#include "compiler/lexer/lexer.h"
#include "compiler/lexer/token_type.h"
#include "compiler/visitor/interfaces/i_visitor.h"
#include "compiler/utils/prysma_cast.h"
#include <llvm/ADT/SmallVector.h>
#include <llvm/Support/FormatVariadic.h>
#include <vector>

namespace
{
  constexpr int DEFAULT_CLASS_VECTOR_SIZE = 8;
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
              llvm::SmallVectorImpl<INode*>& memberList,
              llvm::SmallVectorImpl<INode*>& builders)
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
      if (newDeclarationFunction != nullptr && newDeclarationFunction->getNom().value == param.classNameToken().value) {
        builders.push_back(node);
        return;
      }
      memberList.push_back(node);
      return;
    }

    throw CompilationError(llvm::formatv("Invalid class member: '{0}'", param.classNameToken().value).str(), Line(param.classNameToken().line), Column(param.classNameToken().column));
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
    llvm::SmallVector<INode*, DEFAULT_CLASS_VECTOR_SIZE> inheritance; // TODO: handle multiple inheritance 
    
    
    consume(tokens, index, TOKEN_BRACE_OPEN, "Expected '{' after the class name.");
    llvm::SmallVector<INode*, DEFAULT_CLASS_VECTOR_SIZE> memberList;
    llvm::SmallVector<INode*, DEFAULT_CLASS_VECTOR_SIZE> builders;
    
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
        TokenClassName tokenClassName;
        tokenClassName.t = classNameToken;
        TokenVisibility tokenVisibility{current_visibility};
        ClassParameters classParameters(tokenClassName, tokenVisibility);

        classifyClassNode(node, classParameters, _contextParser, memberList, builders);
    }

    consume(tokens, index, TOKEN_BRACE_CLOSE, "Expected '}' at the end of the class declaration.");

    return _contextParser.getBuilderTreeInstruction()->allocate<NodeClass>(
        _contextParser.getBuilderTreeInstruction()->allocateArray<INode*>(inheritance), 
        _contextParser.getBuilderTreeInstruction()->allocateArray<INode*>(memberList), 
        _contextParser.getBuilderTreeInstruction()->allocateArray<INode*>(builders), 
        classNameToken
    );
}

#endif /* PARSER_CLASS_CPP */




