#ifndef EXPRESSION_LITTERAL_CPP
#define EXPRESSION_LITTERAL_CPP

#include "Compiler/Math/ExpressionLiteral.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/ContextExpression.h"
#include "Compiler/Lexer/Lexer.h"
#include <vector>

ExpressionLiteral::ExpressionLiteral(ContextExpression& contexteExpression)
    : _contexteExpression(contexteExpression)
{}

ExpressionLiteral::~ExpressionLiteral()
= default;

auto ExpressionLiteral::construire(std::vector<Token>& equation) -> INode*
{
    return _contexteExpression.getBuilderTreeEquation()->allouer<NodeLiteral>(equation[0]);
}

#endif /* EXPRESSION_LITTERAL_CPP */