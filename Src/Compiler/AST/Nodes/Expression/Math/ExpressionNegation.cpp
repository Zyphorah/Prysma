#ifndef EXPRESSION_NEGATION_CPP
#define EXPRESSION_NEGATION_CPP

#include "Compiler/Math/ExpressionNegation.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/ContextExpression.h"
#include "Compiler/Lexer/Lexer.h"
#include <stdexcept>
#include <vector>

ExpressionNegation::ExpressionNegation(ContextExpression& contexteExpression)
    : _contexteExpression(contexteExpression)
{}

ExpressionNegation::~ExpressionNegation()
= default;

auto ExpressionNegation::construire(std::vector<Token>& equation) -> INode*
{
    if (equation.size() < 2) {
        throw std::runtime_error("Error: '!' doit être suivi d'une expression");
    }

    std::vector<Token> operande(equation.begin() + 1, equation.end());
    INode* exprOperande = _contexteExpression.getBuilderTreeEquation()->construire(operande);
    return _contexteExpression.getBuilderTreeEquation()->allouer<NodeNegation>(equation[0], exprOperande);
}

#endif /* EXPRESSION_NEGATION_CPP */