#ifndef EXPRESSION_UNREFVARIABLE_CPP
#define EXPRESSION_UNREFVARIABLE_CPP

#include "Compilateur/Variable/ExpressionUnRefVariable.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/Lexer/TokenType.h"
#include <stdexcept>

ExpressionUnRefVariable::ExpressionUnRefVariable(ContexteExpression& contexteExpression)
    : _contexteExpression(contexteExpression)
{}

ExpressionUnRefVariable::~ExpressionUnRefVariable()
{}

INoeud* ExpressionUnRefVariable::construire(std::vector<Token>& equation)
{
    if (equation.size() < 2 || equation[1].type != TOKEN_IDENTIFIANT) {
        throw std::runtime_error("Erreur: 'unref' doit être suivi d'un identifiant");
    }
    return new (_contexteExpression.arena.Allocate(sizeof(NoeudUnRefVariable), alignof(NoeudUnRefVariable))) NoeudUnRefVariable(equation[1].value);
}

#endif /* EXPRESSION_UNREFVARIABLE_CPP */