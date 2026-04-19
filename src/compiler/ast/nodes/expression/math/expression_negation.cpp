#ifndef EXPRESSION_NEGATION_CPP
#define EXPRESSION_NEGATION_CPP

#include "compiler/math/expression_negation.h"
#include "../../../../../../build/generationCode/include/compiler/ast/ast_genere_copy.txt"
#include "compiler/ast/nodes/interfaces/i_node.h"
#include "compiler/ast/registry/context_expression.h"
#include "compiler/lexer/lexer.h"
#include <stdexcept>
#include <vector>

ExpressionNegation::ExpressionNegation(ContextExpression& expressionContext)
    : _context(expressionContext)
{}

ExpressionNegation::~ExpressionNegation()
= default;

auto ExpressionNegation::build(std::vector<Token>& equation) -> INode*
{
    if (equation.size() < 2) {
        throw std::runtime_error("Error: '!' must be followed by an expression");
    }

    std::vector<Token> operand(equation.begin() + 1, equation.end());
    INode* exprOperand = _context.getBuilderTreeEquation()->build(operand);
    return _context.getBuilderTreeEquation()->allocate<NodeNegation>(equation[0], exprOperand);
}

#endif /* EXPRESSION_NEGATION_CPP */