#ifndef PARSEUR_APPELFONCTION_CPP
#define PARSEUR_APPELFONCTION_CPP

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

INode* ParserCallFunction::parser(std::vector<Token>& tokens, int& index)
{
  const bool callCommeInstruction = index == 0 || tokens[static_cast<size_t>(index - 1)].type != TOKEN_EGAL;

  consommer(tokens, index, TOKEN_CALL, "Error: 'call' attendu");
  Token nomFunction = consommer(tokens, index, TOKEN_IDENTIFIANT, "Error: identifiant de function attendu");
  consommer(tokens, index, TOKEN_PAREN_OUVERTE, "Error: '(' attendue");
  
  IInstruction* nodeCall = _contextParser.getBuilderTreeEquation()->allouer<NodeCallFunction>(nomFunction);
  
  consommerChildBody(tokens, index, nodeCall, _contextParser.getBuilderTreeEquation(), TOKEN_PAREN_FERMEE);

  consommer(tokens, index, TOKEN_PAREN_FERMEE, "Error: ')' attendue");

  if (callCommeInstruction && index < static_cast<int>(tokens.size()) && tokens[static_cast<size_t>(index)].type == TOKEN_POINT_VIRGULE) {
      consommer(tokens, index, TOKEN_POINT_VIRGULE, "Error : ';' attendu à la fin de l'call de function");
  }

  return nodeCall;
}

#endif /* PARSEUR_APPELFONCTION_CPP */




