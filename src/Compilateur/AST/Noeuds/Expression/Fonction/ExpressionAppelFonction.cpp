#ifndef EXPRESSION_APPELFONCTION_CPP
#define EXPRESSION_APPELFONCTION_CPP

#include "Compilateur/Fonction/ExpressionAppelFonction.h"
#include "Compilateur/Fonction/ParseurAppelFonction.h"

ExpressionAppelFonction::ExpressionAppelFonction(ContexteExpression& contexteExpression)
    : _contexteExpression(contexteExpression)
{}

ExpressionAppelFonction::~ExpressionAppelFonction()
{}

INoeud* ExpressionAppelFonction::construire(std::vector<Token>& equation)
{
    ParseurAppelFonction parseurAppel(*_contexteExpression.contextParseur);
    int indexZero = 0;
    return parseurAppel.parser(equation, indexZero);
}

#endif /* EXPRESSION_APPELFONCTION_CPP */