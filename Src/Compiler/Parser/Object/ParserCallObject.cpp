#ifndef PARSER_CALLOBJECT_CPP
#define PARSER_CALLOBJECT_CPP

#include "Compiler/Object/ParserCallObject.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Nodes/Interfaces/IInstruction.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/ContextParser.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include <cstddef>
#include <vector>


ParserCallObject::ParserCallObject(ContextParser& contextParser) 
    : _contextParser(contextParser)
{}

ParserCallObject::~ParserCallObject()
{}

auto ParserCallObject::parse(std::vector<Token>& tokens, int& index) -> INode*
{
  const bool callAsInstruction = index == 0 || tokens[static_cast<size_t>(index - 1)].type != TOKEN_EQUAL;

  consume(tokens, index, TOKEN_CALL, "Error: 'call' expected");
  Token objectName = consume(tokens, index, TOKEN_IDENTIFIER, "Error: object identifier expected");
  consume(tokens, index, TOKEN_DOT, "Error: '.' expected after object name");
  Token methodName = consume(tokens, index, TOKEN_IDENTIFIER, "Error: method identifier expected");
  consume(tokens, index, TOKEN_PAREN_OPEN, "Error: '(' expected");

  IInstruction* nodeCall = _contextParser.getBuilderTreeEquation()->allocate<NodeCallObject>(objectName, methodName);

  consumeChildBody(tokens, index, nodeCall, _contextParser.getBuilderTreeEquation(), TOKEN_PAREN_CLOSE);

  consume(tokens, index, TOKEN_PAREN_CLOSE, "Error: ')' expected");

  if (callAsInstruction && index < static_cast<int>(tokens.size()) && tokens[static_cast<size_t>(index)].type == TOKEN_SEMICOLON) {
      consume(tokens, index, TOKEN_SEMICOLON, "Error: ';' expected at the end of the object call");
  }

  return nodeCall;
}

#endif /* PARSER_CALLOBJECT_CPP */




