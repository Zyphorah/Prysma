#ifndef EXPRESSION_CLASS_CPP
#define EXPRESSION_CLASS_CPP

#include "Compilateur/objet/ExpressionClass.h"

ExpressionClass::ExpressionClass(ContexteExpression& contexteExpression)
    : _contexteExpression(contexteExpression)
{}

ExpressionClass::~ExpressionClass()
{}

INoeud* ExpressionClass::construire(std::vector<Token>& equation)
{
    (void)equation;
    return nullptr;
}

#endif /* EXPRESSION_CLASS_CPP */