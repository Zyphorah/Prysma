#ifndef EXPRESSION_REFVARIABLE_CPP
#define EXPRESSION_REFVARIABLE_CPP

#include "Compilateur/Variable/ExpressionRefVariable.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/Lexer/TokenType.h"
#include <stdexcept>

ExpressionRefVariable::ExpressionRefVariable(ContexteExpression& contexteExpression)
    : _contexteExpression(contexteExpression)
{}

ExpressionRefVariable::~ExpressionRefVariable()
= default;

INoeud* ExpressionRefVariable::construire(std::vector<Token>& equation)
{
    if (equation.size() < 2 || equation[1].type != TOKEN_IDENTIFIANT) {
        throw std::runtime_error("Erreur: 'ref' doit être suivi d'un identifiant");
    }
    return new (_contexteExpression.getArena()->Allocate(sizeof(NoeudRefVariable), alignof(NoeudRefVariable))) NoeudRefVariable(equation[1].value); // NOLINT(cppcoreguidelines-owning-memory)
}

#endif /* EXPRESSION_REFVARIABLE_CPP */