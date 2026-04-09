#ifndef PARSER_ARGFUNCTION_CPP
#define PARSER_ARGFUNCTION_CPP

#include "compiler/function/parser_arg_function.h"
#include "compiler/ast/ast_genere.h"
#include "compiler/ast/nodes/interfaces/i_node.h"
#include "compiler/ast/registry/context_parser.h"
#include "compiler/ast/registry/node_component_registry.h"
#include "compiler/ast/registry/types/i_type.h"
#include "compiler/lexer/lexer.h"
#include "compiler/lexer/token_type.h"
#include <cstddef>
#include <vector>


ParserArgFunction::ParserArgFunction(ContextParser& contextParser) 
    : _contextParser(contextParser)
{}

ParserArgFunction::~ParserArgFunction()
= default;

auto ParserArgFunction::parse(std::vector<Token>& tokens, std::size_t index) -> INode* 
{
    consume(tokens, index, TOKEN_ARG, "Error: token is not 'arg'!");

    IType* type = _contextParser.getTypeParser()->parse(tokens, index);

    Token name = consume(tokens, index, TOKEN_IDENTIFIER, "Error: not an identifier!");

    auto* new_node = _contextParser.getBuilderTreeEquation()->allocate<NodeArgFunction>(
        _contextParser.getNodeComponentRegistry()->getNextId()
    );
    
    _contextParser.getNodeComponentRegistry()->insert<AST_ITYPE_COMPONENT>(new_node->getNodeId(), type);
    _contextParser.getNodeComponentRegistry()->insert<AST_NAME_COMPONENT>(new_node->getNodeId(), name);

    return new_node;
}

#endif /* PARSER_ARGFUNCTION_CPP */




