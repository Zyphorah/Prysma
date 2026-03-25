#ifndef PARSEUR_IF_CPP
#define PARSEUR_IF_CPP

#include "Compiler/Instruction/ParserIf.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Nodes/NodeInstruction.h"
#include "Compiler/AST/Registry/ContextParser.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include <cstddef>
#include <vector>


ParserIf::ParserIf(ContextParser& contextParser) 
    : _contextParser(contextParser)
{}

ParserIf::~ParserIf()
= default;

auto ParserIf::parser(std::vector<Token>& tokens, int& index) -> INode* 
{
  consommer(tokens,index,TOKEN_SI,"Error, le token n'est pas 'if'! ");

  consommer(tokens,index,TOKEN_PAREN_OUVERTE,"Error, le token n'est pas '('! ");
  

  INode* condition = _contextParser.getBuilderTreeEquation()->construire(tokens, index);

  consommer(tokens,index,TOKEN_PAREN_FERMEE,"Error, le token n'est pas ')'! ");

  // Créer le node bloc IF
  auto* nodeBlocIf = _contextParser.getBuilderTreeInstruction()->allouer<NodeInstruction>();
  consommer(tokens,index,TOKEN_ACCOLADE_OUVERTE, "Error, le token n'est pas '{'");
  consommerChildBody(tokens,index,nodeBlocIf,_contextParser.getBuilderTreeInstruction(),TOKEN_ACCOLADE_FERMEE);
  consommer(tokens,index,TOKEN_ACCOLADE_FERMEE,"Error, le token n'est pas '}'");

  // Créer le node bloc ELSE s'il existe
  NodeInstruction* nodeBlocElse = nullptr;
  if (index < static_cast<int>(tokens.size()) && tokens[static_cast<size_t>(index)].type == TOKEN_SINON) {
      consommer(tokens,index,TOKEN_SINON,"Error, le token n'est pas 'else'! ");
      nodeBlocElse = _contextParser.getBuilderTreeInstruction()->allouer<NodeInstruction>();
      consommer(tokens,index,TOKEN_ACCOLADE_OUVERTE, "Error, le token n'est pas '{'");
      consommerChildBody(tokens,index,nodeBlocElse,_contextParser.getBuilderTreeInstruction(),TOKEN_ACCOLADE_FERMEE);
      consommer(tokens,index,TOKEN_ACCOLADE_FERMEE,"Error, le token n'est pas '}'");
  }

  // Créer le node bloc ENDIF
  auto* nodeBlocEndif = _contextParser.getBuilderTreeInstruction()->allouer<NodeInstruction>();

  auto* nodeIf = _contextParser.getBuilderTreeInstruction()->allouer<NodeIf>(condition, nodeBlocIf, nodeBlocElse, nodeBlocEndif);

  return nodeIf;
}

#endif /* PARSEUR_IF_CPP */




