#ifndef EXPRESSION_LITERAL_CPP
#define EXPRESSION_LITERAL_CPP

#include "Compiler/Math/ExpressionLiteral.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/ContextExpression.h"
#include "Compiler/Lexer/Lexer.h"
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