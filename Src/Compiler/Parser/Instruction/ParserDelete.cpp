#ifndef PARSER_DELETE_CPP
#define PARSER_DELETE_CPP

#include "Compiler/Instruction/ParserDelete.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/ContextParser.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include <vector>


ParserDelete::ParserDelete(ContextParser& contextParser) 
    : _contextParser(contextParser)
{}

ParserDelete::~ParserDelete()
= default;

// Example: delete variableName;
auto ParserDelete::parse(std::vector<Token>& tokens, int& index) -> INode*
{
    consume(tokens, index, TOKEN_DELETE, "Expected 'delete' at the beginning of the delete instruction.");
    Token identifierToken = consume(tokens, index, TOKEN_IDENTIFIER, "Expected an identifier after 'delete'.");
    consume(tokens, index, TOKEN_SEMICOLON, "Expected ';' after the identifier in the delete instruction.");

    return _contextParser.getBuilderTreeInstruction()->allocate<NodeDelete>(identifierToken);
}

#endif /* PARSER_DELETE_CPP */




