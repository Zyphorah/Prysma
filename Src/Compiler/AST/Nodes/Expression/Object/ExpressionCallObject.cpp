#ifndef EXPRESSION_APPELOBJET_CPP
#define EXPRESSION_APPELOBJET_CPP

#include "Compiler/Object/ExpressionCallObject.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/ContextExpression.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Object/ParserCallObject.h"
#include <vector>

ExpressionCallObject::ExpressionCallObject(ContextExpression& contexteExpression)
    : _contexteExpression(contexteExpression)
{}

ExpressionCallObject::~ExpressionCallObject()
= default;
// Exemple : call object.methode() ou call object.methode(arg int64 param1,arg int64 param2)
auto ExpressionCallObject::construire(std::vector<Token>& equation) -> INode*
{
    ParserCallObject parserCall(*_contexteExpression.getContextParser());
    int indexZero = 0;
    return parserCall.parser(equation, indexZero);
}

#endif /* EXPRESSION_APPELOBJET_CPP */