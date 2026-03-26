#ifndef PARSER_INCLUDE_CPP
#define PARSER_INCLUDE_CPP

#include "Compiler/Instruction/ParserInclude.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/ContextParser.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include <vector>


ParserInclude::ParserInclude(ContextParser& contextParser) 
    : _contextParser(contextParser)
{}

ParserInclude::~ParserInclude()
= default;

// Example: include "path"
auto ParserInclude::parse(std::vector<Token>& tokens, int& index) -> INode*
{
    consume(tokens, index, TOKEN_INCLUDE, "Error: Include instruction must start with the 'include' keyword");
    consume(tokens, index, TOKEN_QUOTE, "Error: Include instruction must be followed by a string in quotes");
    Token tokenPath = consume(tokens, index, TOKEN_IDENTIFIER, "Error: Include instruction must contain a file path");
    consume(tokens, index, TOKEN_QUOTE, "Error: Include instruction must be followed by a string in quotes");
    consume(tokens, index, TOKEN_SEMICOLON, "Error: Include instruction must end with a semicolon");

    return _contextParser.getBuilderTreeEquation()->allocate<NodeInclude>(tokenPath.value);
}

#endif /* PARSER_INCLUDE_CPP */




