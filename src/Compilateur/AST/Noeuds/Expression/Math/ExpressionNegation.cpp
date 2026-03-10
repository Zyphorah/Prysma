#ifndef EXPRESSION_NEGATION_CPP
#define EXPRESSION_NEGATION_CPP

#include "Compilateur/Math/ExpressionNegation.h"
#include "Compilateur/AST/AST_Genere.h"
#include <stdexcept>

ExpressionNegation::ExpressionNegation(ContexteExpression& contexteExpression)
    : _contexteExpression(contexteExpression)
{}

ExpressionNegation::~ExpressionNegation()
{}

INoeud* ExpressionNegation::construire(std::vector<Token>& equation)
{
    if (equation.size() < 2) {
        throw std::runtime_error("Erreur: '!' doit être suivi d'une expression");
    }

    std::vector<Token> operande(equation.begin() + 1, equation.end());
    INoeud* exprOperande = _contexteExpression.constructeurArbreEquation->construire(operande);
    return _contexteExpression.constructeurArbreEquation->allouer<NoeudNegation>(equation[0], exprOperande);
}

#endif /* EXPRESSION_NEGATION_CPP */