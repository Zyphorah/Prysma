#ifndef EXPRESSION_APPELFONCTION_CPP
#define EXPRESSION_APPELFONCTION_CPP

#include "Compiler/Function/ExpressionCallFunction.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/ContextExpression.h"
#include "Compiler/Function/ParserCallFunction.h"
#include "Compiler/Lexer/Lexer.h"
#include <vector>

ExpressionCallFunction::ExpressionCallFunction(ContextExpression& contexteExpression)
    : _contexteExpression(contexteExpression)
{}

ExpressionCallFunction::~ExpressionCallFunction()
= default;

auto ExpressionCallFunction::construire(std::vector<Token>& equation) -> INode*
{
    ParserCallFunction parserCall(*_contexteExpression.getContextParser());
    int indexZero = 0;
    return parserCall.parser(equation, indexZero);
}

#endif /* EXPRESSION_APPELFONCTION_CPP */