#ifndef PARSER_ASSIGNMENTVARIABLE_CPP
#define PARSER_ASSIGNMENTVARIABLE_CPP

#include "Compiler/Variable/ParserAssignmentVariable.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/ContextParser.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include <cstddef>
#include <string>
#include <vector>


ParserAssignmentVariable::ParserAssignmentVariable(ContextParser& contextParser) 
    : _contextParser(contextParser)
{}

ParserAssignmentVariable::~ParserAssignmentVariable()
= default;

auto ParserAssignmentVariable::parse(std::vector<Token>& tokens, int& index) -> INode*
{
    consume(tokens, index, TOKEN_ASSIGN, "Error: 'aff' expected");
    Token nameToken = consume(tokens, index, TOKEN_IDENTIFIER, "Error: variable name expected");
    std::string variableName = nameToken.value;

    if (tokens[static_cast<size_t>(index)].type == TOKEN_DOT) {
        consume(tokens, index, TOKEN_DOT, "Error '.'");
        Token attributeToken = consume(tokens, index, TOKEN_IDENTIFIER, "Error: attribute expected");
        variableName += "." + attributeToken.value;
        nameToken.value = variableName;
    }

    INode* indexExpression = nullptr;

    if (tokens[static_cast<size_t>(index)].type == TOKEN_BRACKET_OPEN) {
        consume(tokens, index, TOKEN_BRACKET_OPEN, "Error '['");
        indexExpression = _contextParser.getBuilderTreeEquation()->build(tokens, index);
        consume(tokens, index, TOKEN_BRACKET_CLOSE, "Error: ']' expected after index");
    }

    consume(tokens, index, TOKEN_EQUAL, "Error: '=' expected");
    INode* expression = _contextParser.getBuilderTreeEquation()->build(tokens, index);
    consume(tokens, index, TOKEN_SEMICOLON, "Error: ';' expected");

    if (indexExpression != nullptr) {
        return _contextParser.getBuilderTreeEquation()->allocate<NodeAssignmentArray>(variableName, indexExpression, expression, nameToken);
    }
    
    return _contextParser.getBuilderTreeEquation()->allocate<NodeAssignmentVariable>(variableName, expression, nameToken);
}

#endif /* PARSER_ASSIGNMENTVARIABLE_CPP */




