#ifndef EXPRESSION_LITTERAL_CPP
#define EXPRESSION_LITTERAL_CPP

#include "Compilateur/Math/ExpressionLitteral.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Registre/ContexteExpression.h"
#include "Compilateur/Lexer/Lexer.h"
#include <vector>

ExpressionLitteral::ExpressionLitteral(ContexteExpression& contexteExpression)
    : _contexteExpression(contexteExpression)
{}

ExpressionLitteral::~ExpressionLitteral()
= default;

auto ExpressionLitteral::construire(std::vector<Token>& equation) -> INoeud*
{
    return _contexteExpression.getConstructeurArbreEquation()->allouer<NoeudLitteral>(equation[0]);
}

#endif /* EXPRESSION_LITTERAL_CPP */