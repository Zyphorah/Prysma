#ifndef PARSER_ASSIGNMENTVARIABLE_CPP
#define PARSER_ASSIGNMENTVARIABLE_CPP

#include "compiler/variable/parser_assignment_variable.h"
#include "compiler/ast/ast_genere.h"
#include "compiler/ast/nodes/interfaces/i_node.h"
#include "compiler/ast/registry/context_parser.h"
#include "compiler/ast/registry/node_component_registry.h"
#include "compiler/lexer/lexer.h"
#include "compiler/lexer/token_type.h"
#include <cstddef>
#include <string>
#include <vector>


ParserAssignmentVariable::ParserAssignmentVariable(ContextParser& contextParser) 
    : _contextParser(contextParser)
{}

ParserAssignmentVariable::~ParserAssignmentVariable()
= default;

auto ParserAssignmentVariable::parse(std::vector<Token>& tokens, std::size_t index) -> INode*
{
    consume(tokens, index, TOKEN_ASSIGN, "Error: 'aff' expected");
    Token nameToken = consume(tokens, index, TOKEN_IDENTIFIER, "Error: variable name expected");
    std::string variableName = std::string(nameToken.value);

    if (tokens[index].type == TOKEN_DOT) {
        consume(tokens, index, TOKEN_DOT, "Error '.'");
        Token attributeToken = consume(tokens, index, TOKEN_IDENTIFIER, "Error: attribute expected");
        variableName += "." + std::string(attributeToken.value);
    }

    INode* indexExpression = nullptr;

    if (tokens[index].type == TOKEN_BRACKET_OPEN) {
        consume(tokens, index, TOKEN_BRACKET_OPEN, "Error '['");
        indexExpression = _contextParser.getBuilderTreeEquation()->build(tokens, index);
        consume(tokens, index, TOKEN_BRACKET_CLOSE, "Error: ']' expected after index");
    }

    consume(tokens, index, TOKEN_EQUAL, "Error: '=' expected");
    INode* expression = _contextParser.getBuilderTreeEquation()->build(tokens, index);
    consume(tokens, index, TOKEN_SEMICOLON, "Error: ';' expected");

    Token constructedToken = nameToken;

    if (indexExpression != nullptr) {
        auto* nodeAssignmentArr = _contextParser.getBuilderTreeInstruction()->allocate<NodeAssignmentArray>(
            _contextParser.getNodeComponentRegistry()->getNextId()
        ); 
        _contextParser.getNodeComponentRegistry()->emplace<NodeAssignmentArrayComponents>(
            nodeAssignmentArr->getNodeId(), constructedToken, indexExpression, expression, nameToken
        );

        return nodeAssignmentArr;
    }

    auto* nodeAssignmentVar = _contextParser.getBuilderTreeInstruction()->allocate<NodeAssignmentVariable>(
        _contextParser.getNodeComponentRegistry()->getNextId()
    ); 
    _contextParser.getNodeComponentRegistry()->emplace<NodeAssignmentVariableComponents>(
        nodeAssignmentVar->getNodeId(), constructedToken, expression, nameToken
    );

    return nodeAssignmentVar;
}

#endif /* PARSER_ASSIGNMENTVARIABLE_CPP */




