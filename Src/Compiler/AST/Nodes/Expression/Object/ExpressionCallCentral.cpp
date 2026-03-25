#ifndef EXPRESSION_APPELCENTRAL_CPP
#define EXPRESSION_APPELCENTRAL_CPP

#include "Compiler/Object/ExpressionCallCentral.h"

#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/ContextExpression.h"
#include "Compiler/Function/ParserCallFunction.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include "Compiler/Object/ParserCallObject.h"
#include <vector>

ExpressionCallCentral::ExpressionCallCentral(ContextExpression& contexteExpression)
    : _contexteExpression(contexteExpression)
{}

ExpressionCallCentral::~ExpressionCallCentral()
= default;

auto ExpressionCallCentral::construire(std::vector<Token>& equation) -> INode*
{
    int indexZero = 0;

    const bool callObject = equation.size() >= 3
        && equation[0].type == TOKEN_CALL
        && equation[1].type == TOKEN_IDENTIFIANT
        && equation[2].type == TOKEN_POINT;

    if (callObject) {
        ParserCallObject parserCall(*_contexteExpression.getContextParser());
        return parserCall.parser(equation, indexZero);
    }

    ParserCallFunction parserCall(*_contexteExpression.getContextParser());
    return parserCall.parser(equation, indexZero);
}

#endif /* EXPRESSION_APPELCENTRAL_CPP */