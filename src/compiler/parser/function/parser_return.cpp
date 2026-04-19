#ifndef PARSER_RETURN_CPP
#define PARSER_RETURN_CPP

#include "compiler/function/parser_return.h"
#include "../../../../build/generationCode/include/compiler/ast/ast_genere_copy.txt"
#include "compiler/ast/nodes/interfaces/i_node.h"
#include "compiler/ast/registry/context_parser.h"
#include "compiler/ast/registry/node_component_registry.h"
#include "compiler/lexer/lexer.h"
#include "compiler/lexer/token_type.h"
#include <cstddef>
#include <vector>


ParserReturn::ParserReturn(ContextParser& contextParser) 
    : _contextParser(contextParser)
{}

ParserReturn::~ParserReturn() = default;

auto ParserReturn::parse(std::vector<Token>& tokens, std::size_t index) -> INode*
{
    consume(tokens, index, TOKEN_RETURN, "Error: not the correct token! 'return'");

    INode* returnValue = nullptr;

    if (index < tokens.size() && tokens[index].type != TOKEN_SEMICOLON) {
        returnValue = _contextParser.getBuilderTreeEquation()->build(tokens, index);
    } else {
        consume(tokens, index, TOKEN_SEMICOLON, "Error: semicolon expected after return");

        auto* new_node = _contextParser.getBuilderTreeEquation()->allocate<NodeReturn>(
            _contextParser.getNodeComponentRegistry()->getNextId()
        );
        _contextParser.getNodeComponentRegistry()->emplace<NodeReturnComponents>(new_node->getNodeId(), returnValue);

        return new_node;
    }
    
    consume(tokens, index, TOKEN_SEMICOLON, "Error: ';' expected at the end of return");

    auto* new_node = _contextParser.getBuilderTreeEquation()->allocate<NodeReturn>(
        _contextParser.getNodeComponentRegistry()->getNextId()
    );
    _contextParser.getNodeComponentRegistry()->emplace<NodeReturnComponents>(new_node->getNodeId(), returnValue);

    return new_node;
}

#endif /* PARSER_RETURN_CPP */




