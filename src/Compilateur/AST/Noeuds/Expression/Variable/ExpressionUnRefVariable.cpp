#ifndef EXPRESSION_UNREFVARIABLE_CPP
#define EXPRESSION_UNREFVARIABLE_CPP

#include "Compilateur/Variable/ExpressionUnRefVariable.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Registre/ContexteExpression.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Lexer/TokenType.h"
#include <stdexcept>
#include <vector>

ExpressionUnRefVariable::ExpressionUnRefVariable(ContexteExpression& contexteExpression)
    : _contexteExpression(contexteExpression)
{}

ExpressionUnRefVariable::~ExpressionUnRefVariable()
= default;

auto ExpressionUnRefVariable::construire(std::vector<Token>& equation) -> INoeud*
{
    if (equation.size() < 2 || equation[1].type != TOKEN_IDENTIFIANT) {
        throw std::runtime_error("Erreur: 'unref' doit être suivi d'un identifiant");
    }
    return _contexteExpression.getConstructeurArbreEquation()->allouer<NoeudUnRefVariable>(equation[1].value); 
}

#endif /* EXPRESSION_UNREFVARIABLE_CPP */