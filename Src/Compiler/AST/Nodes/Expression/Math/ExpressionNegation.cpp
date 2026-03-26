#ifndef EXPRESSION_NEGATION_CPP
#define EXPRESSION_NEGATION_CPP

#include "Compiler/Math/ExpressionNegation.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/ContextExpression.h"
#include "Compiler/Lexer/Lexer.h"
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