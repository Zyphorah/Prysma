#ifndef EXPRESSION_APPELCENTRAL_CPP
#define EXPRESSION_APPELCENTRAL_CPP

#include "Compilateur/objet/ExpressionAppelCentral.h"

#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Registre/ContexteExpression.h"
#include "Compilateur/Fonction/ParseurAppelFonction.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/objet/ParseurAppelObjet.h"
#include <vector>

ExpressionAppelCentral::ExpressionAppelCentral(ContexteExpression& contexteExpression)
    : _contexteExpression(contexteExpression)
{}

ExpressionAppelCentral::~ExpressionAppelCentral()
= default;

auto ExpressionAppelCentral::construire(std::vector<Token>& equation) -> INoeud*
{
    int indexZero = 0;

    const bool appelObjet = equation.size() >= 3
        && equation[0].type == TOKEN_CALL
        && equation[1].type == TOKEN_IDENTIFIANT
        && equation[2].type == TOKEN_POINT;

    if (appelObjet) {
        ParseurAppelObjet parseurAppel(*_contexteExpression.getContextParseur());
        return parseurAppel.parser(equation, indexZero);
    }

    ParseurAppelFonction parseurAppel(*_contexteExpression.getContextParseur());
    return parseurAppel.parser(equation, indexZero);
}

#endif /* EXPRESSION_APPELCENTRAL_CPP */