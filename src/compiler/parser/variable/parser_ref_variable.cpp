#ifndef PARSER_REFVARIABLE_CPP
#define PARSER_REFVARIABLE_CPP

#include "compiler/variable/parser_ref_variable.h"
#include "compiler/ast/ast_genere.h"
#include "compiler/ast/nodes/interfaces/i_node.h"
#include "compiler/ast/registry/context_parser.h"
#include "compiler/ast/registry/node_component_registry.h"
#include "compiler/lexer/lexer.h"
#include "compiler/lexer/token_type.h"
#include <cstddef>
#include <string>
#include <vector>


ParserRefVariable::ParserRefVariable(ContextParser& contextParser) 
    : _contextParser(contextParser)
{}

ParserRefVariable::~ParserRefVariable()
=default;

// Example: ref variable
auto ParserRefVariable::parse(std::vector<Token>& tokens, std::size_t index) -> INode* 
{
    consume(tokens, index, TOKEN_REF, "Error: 'ref' expected");
    
    Token nameToken = consume(tokens, index, TOKEN_IDENTIFIER, "Error: variable name expected after 'ref'");

    auto* new_node = _contextParser.getBuilderTreeEquation()->allocate<NodeRefVariable>(
        _contextParser.getNodeComponentRegistry()->getNextId()
    );
    _contextParser.getNodeComponentRegistry()->emplace<NodeRefVariableComponents>(new_node->getNodeId(), nameToken);
    
    return new_node;
}

#endif /* PARSER_REFVARIABLE_CPP */




