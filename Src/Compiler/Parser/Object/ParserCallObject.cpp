#ifndef PARSEUR_APPELOBJET_CPP
#define PARSEUR_APPELOBJET_CPP

#include "Compiler/Object/ParserCallObject.h"
#include "Compiler/AST/AST_Genere.h"
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

auto ParserCallObject::parser(std::vector<Token>& tokens, int& index) -> INode*
{
  const bool callCommeInstruction = index == 0 || tokens[static_cast<size_t>(index - 1)].type != TOKEN_EGAL;

  consommer(tokens, index, TOKEN_CALL, "Error: 'call' attendu");
  Token nomObject = consommer(tokens, index, TOKEN_IDENTIFIANT, "Error: identifiant d'object attendu");
  consommer(tokens, index, TOKEN_POINT, "Error: '.' attendu après le nom de l'object");
  Token nomMethode = consommer(tokens, index, TOKEN_IDENTIFIANT, "Error: identifiant de méthode attendu");
  consommer(tokens, index, TOKEN_PAREN_OUVERTE, "Error: '(' attendue");

  IInstruction* nodeCall = _contextParser.getBuilderTreeEquation()->allouer<NodeCallObject>(nomObject, nomMethode);

  consommerChildBody(tokens, index, nodeCall, _contextParser.getBuilderTreeEquation(), TOKEN_PAREN_FERMEE);

  consommer(tokens, index, TOKEN_PAREN_FERMEE, "Error: ')' attendue");

  if (callCommeInstruction && index < static_cast<int>(tokens.size()) && tokens[static_cast<size_t>(index)].type == TOKEN_POINT_VIRGULE) {
      consommer(tokens, index, TOKEN_POINT_VIRGULE, "Error : ';' attendu à la fin de l'call d'object");
  }

  return nodeCall;
}

#endif /* PARSEUR_APPELOBJET_CPP */




