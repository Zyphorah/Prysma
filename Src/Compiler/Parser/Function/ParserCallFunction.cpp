#ifndef PARSER_CALLFUNCTION_CPP
#define PARSER_CALLFUNCTION_CPP

#include "Compiler/Function/ParserCallFunction.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Nodes/Interfaces/IInstruction.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/ContextParser.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
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
  
  IInstruction* nodeCall = _contextParser.getBuilderTreeEquation()->allocate<NodeCallFunction>(functionName);
  
  consumeChildBody(tokens, index, nodeCall, _contextParser.getBuilderTreeEquation(), TOKEN_PAREN_CLOSE);

  consume(tokens, index, TOKEN_PAREN_CLOSE, "Error: ')' expected");

  if (callAsInstruction && index < static_cast<int>(tokens.size()) && tokens[static_cast<size_t>(index)].type == TOKEN_SEMICOLON) {
      consume(tokens, index, TOKEN_SEMICOLON, "Error: ';' expected at the end of the function call");
  }

  return nodeCall;
}

#endif /* PARSER_CALLFUNCTION_CPP */




