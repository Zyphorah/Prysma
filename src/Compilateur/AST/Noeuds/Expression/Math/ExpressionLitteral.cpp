#ifndef EXPRESSION_LITTERAL_CPP
#define EXPRESSION_LITTERAL_CPP

#include "Compilateur/Math/ExpressionLitteral.h"
#include "Compilateur/AST/AST_Genere.h"

ExpressionLitteral::ExpressionLitteral(ContexteExpression& contexteExpression)
    : _contexteExpression(contexteExpression)
{}

ExpressionLitteral::~ExpressionLitteral()
{}

INoeud* ExpressionLitteral::construire(std::vector<Token>& equation)
{
    return new (_contexteExpression.arena.Allocate(sizeof(NoeudLitteral), alignof(NoeudLitteral))) NoeudLitteral(equation[0]);
}

#endif /* EXPRESSION_LITTERAL_CPP */