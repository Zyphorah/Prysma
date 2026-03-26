#ifndef PARSER_IF_CPP
#define PARSER_IF_CPP

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

auto ParserIf::parse(std::vector<Token>& tokens, int& index) -> INode* 
{
  consume(tokens, index, TOKEN_IF, "Error, token is not 'if'! ");

  consume(tokens, index, TOKEN_PAREN_OPEN, "Error, token is not '('! ");
  
  INode* condition = _contextParser.getBuilderTreeEquation()->build(tokens, index);

  consume(tokens, index, TOKEN_PAREN_CLOSE, "Error, token is not ')'! ");

  // Create the IF block node
  auto* nodeBlockIf = _contextParser.getBuilderTreeInstruction()->allocate<NodeInstruction>();
  consume(tokens, index, TOKEN_BRACE_OPEN, "Error, token is not '{'");
  consumeChildBody(tokens, index, nodeBlockIf, _contextParser.getBuilderTreeInstruction(), TOKEN_BRACE_CLOSE);
  consume(tokens, index, TOKEN_BRACE_CLOSE, "Error, token is not '}'");

  // Create the ELSE block node if it exists
  NodeInstruction* nodeBlockElse = nullptr;
  if (index < static_cast<int>(tokens.size()) && tokens[static_cast<size_t>(index)].type == TOKEN_ELSE) {
      consume(tokens, index, TOKEN_ELSE, "Error, token is not 'else'! ");
      nodeBlockElse = _contextParser.getBuilderTreeInstruction()->allocate<NodeInstruction>();
      consume(tokens, index, TOKEN_BRACE_OPEN, "Error, token is not '{'");
      consumeChildBody(tokens, index, nodeBlockElse, _contextParser.getBuilderTreeInstruction(), TOKEN_BRACE_CLOSE);
      consume(tokens, index, TOKEN_BRACE_CLOSE, "Error, token is not '}'");
  }

  // Create the ENDIF block node
  auto* nodeBlockEndif = _contextParser.getBuilderTreeInstruction()->allocate<NodeInstruction>();

  auto* nodeIf = _contextParser.getBuilderTreeInstruction()->allocate<NodeIf>(condition, nodeBlockIf, nodeBlockElse, nodeBlockEndif);

  return nodeIf;
}

#endif /* PARSER_IF_CPP */




