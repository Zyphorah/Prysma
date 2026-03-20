#ifndef EXPRESSION_NEGATION_CPP
#define EXPRESSION_NEGATION_CPP

#include "Compilateur/Math/ExpressionNegation.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Registre/ContexteExpression.h"
#include "Compilateur/Lexer/Lexer.h"
#include <stdexcept>
#include <vector>

ExpressionNegation::ExpressionNegation(ContexteExpression& contexteExpression)
    : _contexteExpression(contexteExpression)
{}

ExpressionNegation::~ExpressionNegation()
= default;

auto ExpressionNegation::construire(std::vector<Token>& equation) -> INoeud*
{
    if (equation.size() < 2) {
        throw std::runtime_error("Erreur: '!' doit être suivi d'une expression");
    }

    std::vector<Token> operande(equation.begin() + 1, equation.end());
    INoeud* exprOperande = _contexteExpression.getConstructeurArbreEquation()->construire(operande);
    return _contexteExpression.getConstructeurArbreEquation()->allouer<NoeudNegation>(equation[0], exprOperande);
}

#endif /* EXPRESSION_NEGATION_CPP */