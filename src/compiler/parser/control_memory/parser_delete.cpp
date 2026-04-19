#ifndef PARSER_DELETE_CPP
#define PARSER_DELETE_CPP

#include "compiler/control_memory/parser_delete.h"
#include "../../../../build/generationCode/include/compiler/ast/ast_genere_copy.txt"
#include "compiler/ast/nodes/interfaces/i_node.h"
#include "compiler/ast/registry/context_parser.h"
#include "compiler/lexer/lexer.h"
#include "compiler/lexer/token_type.h"
#include <cstddef>
#include <vector>


ParserDelete::ParserDelete(ContextParser& contextParser) 
    : _contextParser(contextParser)
{}

ParserDelete::~ParserDelete()
= default;

// Example: delete variableName;
auto ParserDelete::parse(std::vector<Token>& tokens, std::size_t index) -> INode*
{
    consume(tokens, index, TOKEN_DELETE, "Expected 'delete' at the beginning of the delete instruction.");
    Token identifierToken = consume(tokens, index, TOKEN_IDENTIFIER, "Expected an identifier after 'delete'.");
    consume(tokens, index, TOKEN_SEMICOLON, "Expected ';' after the identifier in the delete instruction.");

    return _contextParser.getBuilderTreeInstruction()->allocate<NodeDelete>(identifierToken);
}

#endif /* PARSER_DELETE_CPP */




