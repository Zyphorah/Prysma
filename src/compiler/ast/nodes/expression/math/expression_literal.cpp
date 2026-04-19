#ifndef EXPRESSION_LITERAL_CPP
#define EXPRESSION_LITERAL_CPP

#include "compiler/math/expression_literal.h"
#include "../../../../../../build/generationCode/include/compiler/ast/ast_genere_copy.txt"
#include "compiler/ast/nodes/interfaces/i_node.h"
#include "compiler/ast/registry/context_expression.h"
#include "compiler/lexer/lexer.h"
#include <vector>

ExpressionLiteral::ExpressionLiteral(ContextExpression& expressionContext)
    : _context(expressionContext)
{}

ExpressionLiteral::~ExpressionLiteral()
= default;

auto ExpressionLiteral::build(std::vector<Token>& equation) -> INode*
{
    return _context.getBuilderTreeEquation()->allocate<NodeLiteral>(equation[0]);
}

#endif /* EXPRESSION_LITERAL_CPP */