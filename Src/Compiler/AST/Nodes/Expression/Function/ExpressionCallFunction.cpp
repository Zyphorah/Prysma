#ifndef EXPRESSION_CALLFUNCTION_CPP
#define EXPRESSION_CALLFUNCTION_CPP

#include "Compiler/Function/ExpressionCallFunction.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/ContextExpression.h"
#include "Compiler/Function/ParserCallFunction.h"
#include "Compiler/Lexer/Lexer.h"
#include <vector>

ExpressionCallFunction::ExpressionCallFunction(ContextExpression& expressionContext)
    : _context(expressionContext)
{}

ExpressionCallFunction::~ExpressionCallFunction()
= default;

auto ExpressionCallFunction::build(std::vector<Token>& equation) -> INode*
{
    ParserCallFunction parserCall(*_context.getContextParser());
    int indexZero = 0;
    return parserCall.parse(equation, indexZero);
}

#endif /* EXPRESSION_CALLFUNCTION_CPP */