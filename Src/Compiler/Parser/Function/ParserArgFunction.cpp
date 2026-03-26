#ifndef PARSER_ARGFUNCTION_CPP
#define PARSER_ARGFUNCTION_CPP

#include "Compiler/Function/ParserArgFunction.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/ContextParser.h"
#include "Compiler/AST/Registry/Types/IType.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include <vector>


ParserArgFunction::ParserArgFunction(ContextParser& contextParser) 
    : _contextParser(contextParser)
{}

ParserArgFunction::~ParserArgFunction()
= default;

auto ParserArgFunction::parse(std::vector<Token>& tokens, int& index) -> INode* 
{
  consume(tokens, index, TOKEN_ARG, "Error: token is not 'arg'!");

  IType* type = _contextParser.getTypeParser()->parse(tokens, index);

  Token name = consume(tokens, index, TOKEN_IDENTIFIER, "Error: not an identifier!");

  return _contextParser.getBuilderTreeEquation()->allocate<NodeArgFunction>(type, name.value);
}

#endif /* PARSER_ARGFUNCTION_CPP */




