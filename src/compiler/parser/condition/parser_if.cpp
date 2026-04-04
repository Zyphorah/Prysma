//===-- parser_if.cpp -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#ifndef PARSER_IF_CPP
#define PARSER_IF_CPP

#include "compiler/condition/parser_if.h"
#include "compiler/ast/ast_genere.h"
#include "compiler/ast/nodes/interfaces/i_node.h"
#include "compiler/ast/registry/context_parser.h"
#include "compiler/lexer/lexer.h"
#include "compiler/lexer/token_type.h"
#include <cstddef>
#include <vector>


ParserIf::ParserIf(ContextParser& contextParser) 
    : _contextParser(contextParser)
{}

ParserIf::~ParserIf()
= default;

auto ParserIf::parse(std::vector<Token>& tokens, int& index) -> INode* 
{
  consume(tokens, index, TOKEN_IF, "Error, token is not 'if'! ");

  consume(tokens, index, TOKEN_PAREN_OPEN, "Error, token is not '('! ");
  
  INode* condition = _contextParser.getBuilderTreeEquation()->build(tokens, index);

  consume(tokens, index, TOKEN_PAREN_CLOSE, "Error, token is not ')'! ");

  // Create the IF block node
  consume(tokens, index, TOKEN_BRACE_OPEN, "Error, token is not '{'");
  auto ifChildren = consumeChildBody(tokens, index, _contextParser.getBuilderTreeInstruction(), TOKEN_BRACE_CLOSE);
  INode* nodeBlockIf = _contextParser.getBuilderTreeInstruction()->allocate<NodeInstruction>(ifChildren);
  consume(tokens, index, TOKEN_BRACE_CLOSE, "Error, token is not '}'");

  // Create the ELSE block node if it exists
  INode* nodeBlockElse = nullptr;
  if (index < static_cast<int>(tokens.size()) && tokens[static_cast<size_t>(index)].type == TOKEN_ELSE) {
      consume(tokens, index, TOKEN_ELSE, "Error, token is not 'else'! ");
      consume(tokens, index, TOKEN_BRACE_OPEN, "Error, token is not '{'");
      auto elseChildren = consumeChildBody(tokens, index, _contextParser.getBuilderTreeInstruction(), TOKEN_BRACE_CLOSE);
      nodeBlockElse = _contextParser.getBuilderTreeInstruction()->allocate<NodeInstruction>(elseChildren);
      consume(tokens, index, TOKEN_BRACE_CLOSE, "Error, token is not '}'");
  }

  // Create the ENDIF block node
  auto* nodeBlockEndif = _contextParser.getBuilderTreeInstruction()->allocate<NodeInstruction>(llvm::ArrayRef<INode*>{});

  auto* nodeIf = _contextParser.getBuilderTreeInstruction()->allocate<NodeIf>(condition, nodeBlockIf, nodeBlockElse, nodeBlockEndif);

  return nodeIf;
}

#endif /* PARSER_IF_CPP */




