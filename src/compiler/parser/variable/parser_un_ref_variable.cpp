#ifndef PARSER_UNREFVARIABLE_CPP
#define PARSER_UNREFVARIABLE_CPP

#include "compiler/variable/parser_un_ref_variable.h"
#include "compiler/ast/ast_genere.h"
#include "compiler/ast/nodes/interfaces/i_node.h"
#include "compiler/ast/registry/context_parser.h"
#include "compiler/ast/registry/node_component_registry.h"
#include "compiler/lexer/lexer.h"
#include "compiler/lexer/token_type.h"
#include <cstddef>
#include <string>
#include <vector>


ParserUnRefVariable::ParserUnRefVariable(ContextParser& contextParser) 
    : _contextParser(contextParser)
{}

ParserUnRefVariable::~ParserUnRefVariable()
= default;

// Example: unref variable
auto ParserUnRefVariable::parse(std::vector<Token>& tokens, std::size_t index) -> INode* 
{
    consume(tokens, index, TOKEN_UNREF, "Error: 'unref' expected");
    
    Token nameToken = consume(tokens, index, TOKEN_IDENTIFIER, "Error: variable name expected after 'unref'");
    
    auto* new_node = _contextParser.getBuilderTreeEquation()->allocate<NodeUnRefVariable>(
        _contextParser.getNodeComponentRegistry()->getNextId()
    );
    _contextParser.getNodeComponentRegistry()->emplace<NodeUnRefVariableComponents>(new_node->getNodeId(), nameToken);

    return new_node;
}

#endif /* PARSER_UNREFVARIABLE_CPP */




