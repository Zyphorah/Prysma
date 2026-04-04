//===-- parser_call_function.cpp -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#ifndef PARSER_CALLFUNCTION_CPP
#define PARSER_CALLFUNCTION_CPP

#include "compiler/function/parser_call_function.h"
#include "compiler/ast/ast_genere.h"
#include "compiler/ast/nodes/interfaces/i_node.h"
#include "compiler/ast/registry/context_parser.h"
#include "compiler/lexer/lexer.h"
#include "compiler/lexer/token_type.h"
#include <cstddef>
#include <vector>


ParserCallFunction::ParserCallFunction(ContextParser& contextParser) 
    : _contextParser(contextParser)
{}

ParserCallFunction::~ParserCallFunction()
= default;

INode* ParserCallFunction::parse(std::vector<Token>& tokens, int& index)
{
  const bool callAsInstruction = index == 0 || tokens[static_cast<size_t>(index - 1)].type != TOKEN_EQUAL;

  consume(tokens, index, TOKEN_CALL, "Error: 'call' expected");
  Token functionName = consume(tokens, index, TOKEN_IDENTIFIER, "Error: function identifier expected");
  consume(tokens, index, TOKEN_PAREN_OPEN, "Error: '(' expected");
  
  auto children = consumeChildBody(tokens, index, _contextParser.getBuilderTreeEquation(), TOKEN_PAREN_CLOSE);
  INode* nodeCall = _contextParser.getBuilderTreeEquation()->allocate<NodeCallFunction>(functionName, children);

  consume(tokens, index, TOKEN_PAREN_CLOSE, "Error: ')' expected");

  if (callAsInstruction && index < static_cast<int>(tokens.size()) && tokens[static_cast<size_t>(index)].type == TOKEN_SEMICOLON) {
      consume(tokens, index, TOKEN_SEMICOLON, "Error: ';' expected at the end of the function call");
  }

  return nodeCall;
}

#endif /* PARSER_CALLFUNCTION_CPP */




