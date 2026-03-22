#ifndef EXPRESSION_REFVARIABLE_CPP
#define EXPRESSION_REFVARIABLE_CPP

#include "Compilateur/Variable/ExpressionRefVariable.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Registre/ContexteExpression.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Lexer/TokenType.h"
#include <stdexcept>
#include <vector>

ExpressionRefVariable::ExpressionRefVariable(ContexteExpression& contexteExpression)
    : _contexteExpression(contexteExpression)
{}

ExpressionRefVariable::~ExpressionRefVariable()
= default;

auto ExpressionRefVariable::construire(std::vector<Token>& equation) -> INoeud*
{
    if (equation.size() < 2 || equation[1].type != TOKEN_IDENTIFIANT) {
        throw std::runtime_error("Erreur: 'ref' doit être suivi d'un identifiant");
    }
    return _contexteExpression.getConstructeurArbreEquation()->allouer<NoeudRefVariable>(equation[1].value); 
}

#endif /* EXPRESSION_REFVARIABLE_CPP */