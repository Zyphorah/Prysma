#ifndef EXPRESSION_APPELFONCTION_CPP
#define EXPRESSION_APPELFONCTION_CPP

#include "Compilateur/Fonction/ExpressionAppelFonction.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Registre/ContexteExpression.h"
#include "Compilateur/Fonction/ParseurAppelFonction.h"
#include "Compilateur/Lexer/Lexer.h"
#include <vector>

ExpressionAppelFonction::ExpressionAppelFonction(ContexteExpression& contexteExpression)
    : _contexteExpression(contexteExpression)
{}

ExpressionAppelFonction::~ExpressionAppelFonction()
= default;

auto ExpressionAppelFonction::construire(std::vector<Token>& equation) -> INoeud*
{
    ParseurAppelFonction parseurAppel(*_contexteExpression.getContextParseur());
    int indexZero = 0;
    return parseurAppel.parser(equation, indexZero);
}

#endif /* EXPRESSION_APPELFONCTION_CPP */