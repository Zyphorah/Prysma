#ifndef PARSER_INCLUDE_CPP
#define PARSER_INCLUDE_CPP

#include "compiler/include_module/parser_include.h"
#include "../../../../build/generationCode/include/compiler/ast/ast_genere_copy.txt"
#include "compiler/ast/nodes/interfaces/i_node.h"
#include "compiler/ast/registry/context_parser.h"
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

    return _contextParser.getBuilderTreeEquation()->allocate<NodeInclude>(tokenPath);
}

#endif /* PARSER_INCLUDE_CPP */




