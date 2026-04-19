#ifndef PARSER_CALLFUNCTION_CPP
#define PARSER_CALLFUNCTION_CPP

#include "compiler/function/parser_call_function.h"
#include "../../../../build/generationCode/include/compiler/ast/ast_genere_copy.txt"
#include "compiler/ast/ast_genere.h"
#include "compiler/ast/nodes/interfaces/i_node.h"
#include "compiler/ast/registry/context_parser.h"
#include "compiler/ast/registry/node_component_registry.h"
#include "compiler/lexer/lexer.h"
#include "compiler/lexer/token_type.h"
#include <cstddef>
#include <vector>


ParserCallFunction::ParserCallFunction(ContextParser& contextParser) 
    : _contextParser(contextParser) {}

ParserCallFunction::~ParserCallFunction() = default;

INode* ParserCallFunction::parse(std::vector<Token>& tokens, std::size_t index)
{
    const bool callAsInstruction = index == 0 || tokens[index - 1].type != TOKEN_EQUAL;

    consume(tokens, index, TOKEN_CALL, "Error: 'call' expected");
    Token functionName = consume(tokens, index, TOKEN_IDENTIFIER, "Error: function identifier expected");
    consume(tokens, index, TOKEN_PAREN_OPEN, "Error: '(' expected");
    
    auto children = consumeChildBody(tokens, index, _contextParser.getBuilderTreeEquation(), TOKEN_PAREN_CLOSE);


    NodeComponentRegistry* registry = _contextParser.getNodeComponentRegistry();
    auto* nodeCall = _contextParser.getBuilderTreeEquation()->allocate<NodeCallFunction>(registry->getNextId());

    _contextParser.getNodeComponentRegistry()->emplace<NodeCallFunctionComponents>(nodeCall->getNodeId(), functionName, children);
    
    consume(tokens, index, TOKEN_PAREN_CLOSE, "Error: ')' expected");

    if (callAsInstruction && index < tokens.size() && tokens[index].type == TOKEN_SEMICOLON) {
        consume(tokens, index, TOKEN_SEMICOLON, "Error: ';' expected at the end of the function call");
    }

    return nodeCall;
}

#endif /* PARSER_CALLFUNCTION_CPP */