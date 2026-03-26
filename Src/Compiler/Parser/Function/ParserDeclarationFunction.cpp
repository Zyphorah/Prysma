#ifndef PARSER_DECLARATIONFUNCTION_CPP
#define PARSER_DECLARATIONFUNCTION_CPP

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

INode* ParserDeclarationFunction::parse(std::vector<Token>& tokens, int& index)
{
  consume(tokens, index, TOKEN_FUNCTION, "Error: not the correct token! 'fn'");

  Token tokenTypeReturn = tokens[static_cast<size_t>(index)];
  IType* typeReturn = _contextParser.getTypeParser()->parse(tokens, index);
  
  Token tokenFunctionName = tokens[static_cast<size_t>(index)];
  std::string functionName = tokenFunctionName.value;
  consume(tokens, index, TOKEN_IDENTIFIER, "Error: invalid identifier, must be a function name");

  // Parse arguments inside parentheses
  consume(tokens, index, TOKEN_PAREN_OPEN, "Error: not an opening parenthesis '('");
  
  std::vector<INode*> arguments;
  while(index < static_cast<int>(tokens.size()) && tokens[static_cast<size_t>(index)].type != TOKEN_PAREN_CLOSE)
  {
      if(tokens[static_cast<size_t>(index)].type == TOKEN_COMMA)
      {
          index++;
          continue;
      }
      
      INode* child = _contextParser.getBuilderTreeInstruction()->build(tokens, index);
      arguments.push_back(child);
  }

  consume(tokens, index, TOKEN_PAREN_CLOSE, "Error: not a closing parenthesis ')'");

  // Parse the body in a strict NodeScope
  consume(tokens, index, TOKEN_BRACE_OPEN, "Error: not an opening brace '{' ");

  auto* body = _contextParser.getBuilderTreeInstruction()->allocate<NodeInstruction>();
  consumeChildBody(tokens, index, body, _contextParser.getBuilderTreeInstruction(), TOKEN_BRACE_CLOSE);

  consume(tokens, index, TOKEN_BRACE_CLOSE, "Error: not a closing brace '}'");

  auto* nodeFunction = 
      _contextParser.getBuilderTreeInstruction()->allocate<NodeDeclarationFunction>(Token{}, typeReturn, functionName, arguments, body);

  return nodeFunction; 
}

#endif /* PARSER_DECLARATIONFUNCTION_CPP */




