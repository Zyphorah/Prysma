#ifndef EXPRESSION_CALLCENTRAL_CPP
#define EXPRESSION_CALLCENTRAL_CPP

#include "Compiler/Object/ExpressionCallCentral.h"

#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/ContextExpression.h"
#include "Compiler/Function/ParserCallFunction.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include "Compiler/Object/ParserCallObject.h"
#include <vector>

ExpressionCallCentral::ExpressionCallCentral(ContextExpression& expressionContext)
    : _context(expressionContext)
{}

ExpressionCallCentral::~ExpressionCallCentral()
= default;

auto ExpressionCallCentral::build(std::vector<Token>& equation) -> INode*
{
    int indexZero = 0;

    const bool callObject = equation.size() >= 3
        && equation[0].type == TOKEN_CALL
        && equation[1].type == TOKEN_IDENTIFIER
        && equation[2].type == TOKEN_DOT;

    if (callObject) {
        ParserCallObject parserCall(*_context.getContextParser());
        return parserCall.parse(equation, indexZero);
    }

    ParserCallFunction parserCall(*_context.getContextParser());
    return parserCall.parse(equation, indexZero);
}

#endif /* EXPRESSION_CALLCENTRAL_CPP */