#ifndef PARSER_INCLUDE_CPP
#define PARSER_INCLUDE_CPP

#include "compiler/include_module/parser_include.h"
#include "compiler/ast/ast_genere.h"
#include "compiler/ast/nodes/interfaces/i_node.h"
#include "compiler/ast/registry/context_parser.h"
#include "compiler/ast/registry/node_component_registry.h"
#include "compiler/lexer/lexer.h"
#include "compiler/lexer/token_type.h"
#include <cstddef>
#include <vector>


ParserInclude::ParserInclude(ContextParser& contextParser) 
    : _contextParser(contextParser)
{}

ParserInclude::~ParserInclude()
= default;

// Example: include "path"
auto ParserInclude::parse(std::vector<Token>& tokens, std::size_t index) -> INode*
{
    consume(tokens, index, TOKEN_INCLUDE, "Error: Include instruction must start with the 'include' keyword");
    consume(tokens, index, TOKEN_QUOTE, "Error: Include instruction must be followed by a string in quotes");
    Token tokenPath = consume(tokens, index, TOKEN_IDENTIFIER, "Error: Include instruction must contain a file path");
    consume(tokens, index, TOKEN_QUOTE, "Error: Include instruction must be followed by a string in quotes");
    consume(tokens, index, TOKEN_SEMICOLON, "Error: Include instruction must end with a semicolon");

    auto* nodeInclude = _contextParser.getBuilderTreeInstruction()->allocate<NodeInclude>(
        _contextParser.getNodeComponentRegistry()->getNextId()
    ); 
    _contextParser.getNodeComponentRegistry()->emplace<NodeIncludeComponents>(nodeInclude->getNodeId(), tokenPath);

    return nodeInclude;
}

#endif /* PARSER_INCLUDE_CPP */




