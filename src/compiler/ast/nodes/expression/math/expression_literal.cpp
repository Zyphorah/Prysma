#ifndef EXPRESSION_LITERAL_CPP
#define EXPRESSION_LITERAL_CPP

#include "compiler/math/expression_literal.h"
#include "compiler/ast/ast_genere.h"
#include "compiler/ast/nodes/interfaces/i_node.h"
#include "compiler/ast/registry/context_expression.h"
#include "compiler/ast/registry/node_component_registry.h"
#include "compiler/lexer/lexer.h"
#include <vector>

ExpressionLiteral::ExpressionLiteral(ContextExpression& expressionContext)
    : _context(expressionContext)
{}

ExpressionLiteral::~ExpressionLiteral()
= default;

auto ExpressionLiteral::build(std::vector<Token>& equation) -> INode*
{
    auto* nodeLiteral = _context.getBuilderTreeEquation()->allocate<NodeLiteral>(
        _context.getNodeComponentRegistry()->getNextId()
    );

    _context.getNodeComponentRegistry()->emplace<NodeLiteralComponents>(
        nodeLiteral->getNodeId(), equation[0]
    );

    return nodeLiteral;
}

#endif /* EXPRESSION_LITERAL_CPP */