#ifndef PARSER_RETURN_CPP
#define PARSER_RETURN_CPP

#include "Compiler/Function/ParserReturn.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/ContextParser.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include <cstddef>
#include <vector>


ParserReturn::ParserReturn(ContextParser& contextParser) 
    : _contextParser(contextParser)
{}

ParserReturn::~ParserReturn() = default;

auto ParserReturn::parse(std::vector<Token>& tokens, int& index) -> INode*
{
  consume(tokens, index, TOKEN_RETURN, "Error: not the correct token! 'return'");

  INode* returnValue = nullptr;

  if (index < static_cast<int>(tokens.size()) && tokens[static_cast<size_t>(index)].type != TOKEN_SEMICOLON) {
      returnValue = _contextParser.getBuilderTreeEquation()->build(tokens, index);
  } else {
      consume(tokens, index, TOKEN_SEMICOLON, "Error: semicolon expected after return");
      return _contextParser.getBuilderTreeEquation()->allocate<NodeReturn>(returnValue);
  }
  
  consume(tokens, index, TOKEN_SEMICOLON, "Error: ';' expected at the end of return");

  return _contextParser.getBuilderTreeEquation()->allocate<NodeReturn>(returnValue);
}

#endif /* PARSER_RETURN_CPP */




