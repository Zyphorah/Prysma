#ifndef PARSEUR_ARGFONCTION_CPP
#define PARSEUR_ARGFONCTION_CPP

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

auto ParserArgFunction::parser(std::vector<Token>& tokens, int& index) -> INode* 
{

  consommer(tokens,index,TOKEN_ARG,"Error: le token n'est pas 'arg' !");

  IType* type = _contextParser.getParserType()->parser(tokens, index);

  Token nom = consommer(tokens,index,TOKEN_IDENTIFIANT,"Error: ce n'est pas un identifiant!");

  return _contextParser.getBuilderTreeEquation()->allouer<NodeArgFunction>(type, nom.value);
}

#endif /* PARSEUR_ARGFONCTION_CPP */




