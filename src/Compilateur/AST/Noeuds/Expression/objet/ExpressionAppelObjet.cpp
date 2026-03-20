#ifndef EXPRESSION_APPELOBJET_CPP
#define EXPRESSION_APPELOBJET_CPP

#include "Compilateur/objet/ExpressionAppelObjet.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Registre/ContexteExpression.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/objet/ParseurAppelObjet.h"
#include <vector>

ExpressionAppelObjet::ExpressionAppelObjet(ContexteExpression& contexteExpression)
    : _contexteExpression(contexteExpression)
{}

ExpressionAppelObjet::~ExpressionAppelObjet()
= default;
// Exemple : call objet.methode() ou call objet.methode(arg int64 param1,arg int64 param2)
auto ExpressionAppelObjet::construire(std::vector<Token>& equation) -> INoeud*
{
    ParseurAppelObjet parseurAppel(*_contexteExpression.getContextParseur());
    int indexZero = 0;
    return parseurAppel.parser(equation, indexZero);
}

#endif /* EXPRESSION_APPELOBJET_CPP */