//===-- parser_declaration_function.cpp -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#ifndef PARSER_DECLARATIONFUNCTION_CPP
#define PARSER_DECLARATIONFUNCTION_CPP

#include "compiler/function/parser_declaration_function.h"
#include "compiler/ast/ast_genere.h"
#include "compiler/ast/nodes/interfaces/i_node.h"
#include "compiler/ast/registry/context_parser.h"
#include "compiler/ast/registry/types/i_type.h"
#include "compiler/lexer/lexer.h"
#include "compiler/lexer/token_type.h"
#include <llvm/ADT/SmallVector.h>
#include <cstddef>
#include <string>
#include <vector>


ParserDeclarationFunction::ParserDeclarationFunction(ContextParser& contextParser) 
    : _contextParser(contextParser)
{}

ParserDeclarationFunction::~ParserDeclarationFunction() = default;

auto ParserDeclarationFunction::parse(std::vector<Token>& tokens, int& index) -> INode*
{
  consume(tokens, index, TOKEN_FUNCTION, "Error: not the correct token! 'fn'");

  Token tokenTypeReturn = tokens[static_cast<size_t>(index)];
  IType* typeReturn = _contextParser.getTypeParser()->parse(tokens, index);
  
  Token tokenFunctionName = tokens[static_cast<size_t>(index)];
  consume(tokens, index, TOKEN_IDENTIFIER, "Error: invalid identifier, must be a function name");

  // Parse arguments inside parentheses
  consume(tokens, index, TOKEN_PAREN_OPEN, "Error: not an opening parenthesis '('");
  
  llvm::SmallVector<INode*, 4> arguments;
  while(index < static_cast<int>(tokens.size()) && tokens[static_cast<size_t>(index)].type != TOKEN_PAREN_CLOSE)
  {
      if(tokens[static_cast<size_t>(index)].type == TOKEN_COMMA)
      {
          index++;
          continue;
      }
      
      INode* child = _contextParser.getBuilderTreeInstruction()->build(tokens, index);
      arguments.push_back(child);
  }

  consume(tokens, index, TOKEN_PAREN_CLOSE, "Error: not a closing parenthesis ')'");

  // Parse the body in a strict NodeScope
  consume(tokens, index, TOKEN_BRACE_OPEN, "Error: not an opening brace '{' ");

  auto bodyChildren = consumeChildBody(tokens, index, _contextParser.getBuilderTreeInstruction(), TOKEN_BRACE_CLOSE);
  auto* body = _contextParser.getBuilderTreeInstruction()->allocate<NodeInstruction>(bodyChildren);

  consume(tokens, index, TOKEN_BRACE_CLOSE, "Error: not a closing brace '}'");

  auto* nodeFunction = 
      _contextParser.getBuilderTreeInstruction()->allocate<NodeDeclarationFunction>(Token{}, typeReturn, tokenFunctionName, _contextParser.getBuilderTreeInstruction()->allocateArray<INode*>(arguments), body);

  return nodeFunction; 
}

#endif /* PARSER_DECLARATIONFUNCTION_CPP */




