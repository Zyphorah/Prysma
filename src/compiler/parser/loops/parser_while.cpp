#ifndef PARSER_WHILE_CPP
#define PARSER_WHILE_CPP

#include "compiler/loops/parser_while.h"
#include "compiler/ast/ast_genere.h"
#include "compiler/ast/nodes/interfaces/i_node.h"
#include "compiler/ast/registry/context_parser.h"
#include "compiler/ast/registry/node_component_registry.h"
#include "compiler/lexer/lexer.h"
#include "compiler/lexer/token_type.h"
#include "compiler/visitor/interfaces/i_visitor.h"
#include <cstddef>
#include <llvm/ADT/ArrayRef.h>
#include <vector>


ParserWhile::ParserWhile(ContextParser& contextParser) 
    : _contextParser(contextParser)
{}

ParserWhile::~ParserWhile() = default;

auto ParserWhile::parse(std::vector<Token>& tokens, std::size_t index) -> INode*
{
    consume(tokens, index, TOKEN_WHILE, "Error, expected token 'while' ");

    consume(tokens, index, TOKEN_PAREN_OPEN, "Error, token is not '('! ");
    
    INode* condition = _contextParser.getBuilderTreeEquation()->build(tokens, index);

    consume(tokens, index, TOKEN_PAREN_CLOSE, "Error, token is not ')'! ");

    consume(tokens, index, TOKEN_BRACE_OPEN, "Error, token is not '{'");


    auto blockWhileChildren = consumeChildBody(tokens, index, _contextParser.getBuilderTreeInstruction(), TOKEN_BRACE_CLOSE);

    auto* nodeBlockWhile = _contextParser.getBuilderTreeInstruction()->allocate<NodeInstruction>(
        _contextParser.getNodeComponentRegistry()->getNextId()
    ); 
    _contextParser.getNodeComponentRegistry()->insert<AST_CHILD_COMPONENT>(nodeBlockWhile->getNodeId(), blockWhileChildren);


    consume(tokens, index, TOKEN_BRACE_CLOSE, "Error, token is not '}'");


    auto* nodeBlockEndWhile = _contextParser.getBuilderTreeInstruction()->allocate<NodeInstruction>(
        _contextParser.getNodeComponentRegistry()->getNextId()
    ); 
    _contextParser.getNodeComponentRegistry()->insert<AST_CHILD_COMPONENT>(nodeBlockEndWhile->getNodeId(), llvm::ArrayRef<INode*>{});
    
    
    INode* nodeWhile = _contextParser.getBuilderTreeInstruction()->allocate<NodeWhile>(condition, nodeBlockWhile, nodeBlockEndWhile);

    return nodeWhile;
}

#endif /* PARSER_WHILE_CPP */




