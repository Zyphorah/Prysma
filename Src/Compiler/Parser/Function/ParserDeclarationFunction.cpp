#ifndef PARSEUR_DECLARATIONFONCTION_CPP
#define PARSEUR_DECLARATIONFONCTION_CPP

#include "Compiler/Function/ParserDeclarationFunction.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Nodes/NodeInstruction.h"
#include "Compiler/AST/Registry/ContextParser.h"
#include "Compiler/AST/Registry/Types/IType.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include <cstddef>
#include <string>
#include <vector>


ParserDeclarationFunction::ParserDeclarationFunction(ContextParser& contextParser) 
    : _contextParser(contextParser)
{}

ParserDeclarationFunction::~ParserDeclarationFunction() = default;

INode* ParserDeclarationFunction::parser(std::vector<Token>& tokens, int& index)
{
  consommer(tokens, index, TOKEN_FONCTION, "Error: ce n'est pas le bon token ! 'fn'");

  Token tokenTypeReturn = tokens[static_cast<size_t>(index)];
  IType* typeReturn = _contextParser.getParserType()->parser(tokens, index);
  
  Token tokenNomFunction = tokens[static_cast<size_t>(index)];
  std::string nomFunction = tokenNomFunction.value;
  consommer(tokens, index, TOKEN_IDENTIFIANT, "Error: identifiant invalide, ce dois être un nom de function ");

  // Parser les arguments input parenthèses
  consommer(tokens, index, TOKEN_PAREN_OUVERTE, "Error: ce n'est pas une parenthèse ouverte '('");
  
  std::vector<INode*> arguments;
  while(index < static_cast<int>(tokens.size()) && tokens[static_cast<size_t>(index)].type != TOKEN_PAREN_FERMEE)
  {
      if(tokens[static_cast<size_t>(index)].type == TOKEN_VIRGULE)
      {
          index++;
          continue;
      }
      
      INode* child = _contextParser.getBuilderTreeInstruction()->construire(tokens, index);
      arguments.push_back(child);
  }

  consommer(tokens, index, TOKEN_PAREN_FERMEE, "Error: ce n'est pas une parenthèse fermée ')'");

  // Parser le body dans un NodeScope strict
  consommer(tokens, index, TOKEN_ACCOLADE_OUVERTE, "Error: ce n'est pas une accolade ouverte '{' ");

  auto* body = _contextParser.getBuilderTreeInstruction()->allouer<NodeInstruction>();
  consommerChildBody(tokens, index, body, _contextParser.getBuilderTreeInstruction(), TOKEN_ACCOLADE_FERMEE);

  consommer(tokens, index, TOKEN_ACCOLADE_FERMEE, "Error: ce n'est pas une accolade fermée '}'");

  auto* nodeFunction = 
      _contextParser.getBuilderTreeInstruction()->allouer<NodeDeclarationFunction>(Token{}, typeReturn, nomFunction, arguments, body);

  return nodeFunction; 
}

#endif /* PARSEUR_DECLARATIONFONCTION_CPP */




