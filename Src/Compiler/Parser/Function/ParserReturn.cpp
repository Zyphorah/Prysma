#ifndef PARSEUR_RETOUR_CPP
#define PARSEUR_RETOUR_CPP

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

auto ParserReturn::parser(std::vector<Token>& tokens, int& index) -> INode*
{
  consommer(tokens, index, TOKEN_RETOUR, "Error: ce n'est pas le bon token ! 'return'");

  INode* valeurReturn = nullptr;

  if (index < static_cast<int>(tokens.size()) && tokens[static_cast<size_t>(index)].type != TOKEN_POINT_VIRGULE) {
      valeurReturn = _contextParser.getBuilderTreeEquation()->construire(tokens, index);
  } else {
      consommer(tokens, index, TOKEN_POINT_VIRGULE, "Error: point-virgule attendu après return");
      return _contextParser.getBuilderTreeEquation()->allouer<NodeReturn>(valeurReturn);
  }
  
  consommer(tokens, index, TOKEN_POINT_VIRGULE, "Error : ';' attendu à la fin du return");

  return _contextParser.getBuilderTreeEquation()->allouer<NodeReturn>(valeurReturn);
}

#endif /* PARSEUR_RETOUR_CPP */




