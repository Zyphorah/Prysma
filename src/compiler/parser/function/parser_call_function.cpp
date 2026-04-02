#ifndef PARSER_CALLFUNCTION_CPP
#define PARSER_CALLFUNCTION_CPP

#include "compiler/function/parser_call_function.h"
#include "compiler/ast/ast_genere.h"
#include "compiler/ast/nodes/interfaces/i_node.h"
#include "compiler/ast/registry/context_parser.h"
#include "compiler/ast/registry/node_component_registry.h"
#include "compiler/lexer/lexer.h"
#include "compiler/lexer/token_type.h"
#include <cstddef>
#include <vector>


ParserCallFunction::ParserCallFunction(ContextParser& contextParser) 
    : _contextParser(contextParser)
{}

ParserCallFunction::~ParserCallFunction()
= default;

INode* ParserCallFunction::parse(std::vector<Token>& tokens, int& index)
{
  const bool callAsInstruction = index == 0 || tokens[static_cast<size_t>(index - 1)].type != TOKEN_EQUAL;

  consume(tokens, index, TOKEN_CALL, "Error: 'call' expected");
  Token functionName = consume(tokens, index, TOKEN_IDENTIFIER, "Error: function identifier expected");
  consume(tokens, index, TOKEN_PAREN_OPEN, "Error: '(' expected");
  
  auto children = consumeChildBody(tokens, index, _contextParser.getBuilderTreeEquation(), TOKEN_PAREN_CLOSE);

  /////////
  // ->  INode* nodeCall = _contextParser.getBuilderTreeEquation()->allocate<NodeCallFunction>(functionName, children);
  // Il allocate l'objet node dans arena alloc. Il s'agirait de toujours effectuer l'allocation du node dans arena alloc mais 
  // de gérer ses composantes hors de celui-ci. Donc le noeud reste dans l'arena et ses composantes sont dans un autre registre.
  // De cette façon, tous les noeuds sont contigues dans le pool. Chacuns des noeuds stockent un ID. Ce ID permet l'accès aux composantes
  // elles aussi contigues dans les sparse sets du registre de composantes de noeuds. C'est ultra performant ;)
  /////////

  NodeComponentRegistry* registry = _contextParser.getNodeComponentRegistry();
  auto* nodeCall = _contextParser.getBuilderTreeEquation()->allocate<NodeCallFunction>(registry->getNextId());

  registry->insert<NAME_COMPONENT_TAG>(nodeCall->getNodeId(), functionName);
  registry->insert<CHILD_COMPONENT_TAG>(nodeCall->getNodeId(), children);

  //NodeCallFunction(Token nomFunction, llvm::ArrayRef<INode*> children = nullptr) : _nomFunction(std::move(nomFunction)), _children(children) {}

  /////////

  consume(tokens, index, TOKEN_PAREN_CLOSE, "Error: ')' expected");

  if (callAsInstruction && index < static_cast<int>(tokens.size()) && tokens[static_cast<size_t>(index)].type == TOKEN_SEMICOLON) {
      consume(tokens, index, TOKEN_SEMICOLON, "Error: ';' expected at the end of the function call");
  }

  return nodeCall;
}

#endif /* PARSER_CALLFUNCTION_CPP */




