#ifndef EXPRESSION_CALLOBJECT_CPP
#define EXPRESSION_CALLOBJECT_CPP

#include "Compiler/Object/ExpressionCallObject.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/ContextExpression.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Object/ParserCallObject.h"
#include <vector>

ExpressionCallObject::ExpressionCallObject(ContextExpression& expressionContext)
    : _context(expressionContext)
{}

ExpressionCallObject::~ExpressionCallObject()
= default;
// Example: call object.method() or call object.method(arg int64 param1, arg int64 param2)
auto ExpressionCallObject::build(std::vector<Token>& equation) -> INode*
{
    ParserCallObject parserCall(*_context.getContextParser());
    int indexZero = 0;
    return parserCall.parse(equation, indexZero);
}

#endif /* EXPRESSION_CALLOBJECT_CPP */