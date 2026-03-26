#ifndef PARSER_UNREFVARIABLE_CPP
#define PARSER_UNREFVARIABLE_CPP

#include "Compiler/Variable/ParserUnRefVariable.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/ContextParser.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include <string>
#include <vector>


ParserUnRefVariable::ParserUnRefVariable(ContextParser& contextParser) 
    : _contextParser(contextParser)
{}

ParserUnRefVariable::~ParserUnRefVariable()
= default;

// Example: unref variable
INode* ParserUnRefVariable::parse(std::vector<Token>& tokens, int& index) 
{
    consume(tokens, index, TOKEN_UNREF, "Error: 'unref' expected");
    
    Token nameToken = consume(tokens, index, TOKEN_IDENTIFIER, "Error: variable name expected after 'unref'");
    std::string variableName = nameToken.value;
    
    return _contextParser.getBuilderTreeEquation()->allocate<NodeUnRefVariable>(variableName);
}

#endif /* PARSER_UNREFVARIABLE_CPP */




