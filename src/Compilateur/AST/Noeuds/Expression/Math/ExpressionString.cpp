#ifndef EXPRESSION_STRING_CPP
#define EXPRESSION_STRING_CPP

#include "Compilateur/Math/ExpressionString.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Registre/ContexteExpression.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Lexer/TokenType.h"
#include <cstddef>
#include <stdexcept>
#include <string>
#include <vector>

ExpressionString::ExpressionString(ContexteExpression& contexteExpression)
    : _contexteExpression(contexteExpression)
{}

ExpressionString::~ExpressionString()
= default;

auto ExpressionString::construire(std::vector<Token>& equation) -> INoeud*
{
    std::vector<INoeud*> elementsString;

    int index = 0;
    if (equation.empty() || equation[0].type != TOKEN_GUILLEMET) {
        throw std::runtime_error("Erreur: une chaine de caractere doit commencer par un guillemet");
    }
    index++;

    if (index >= static_cast<int>(equation.size()) || equation[static_cast<size_t>(index)].type != TOKEN_IDENTIFIANT) {
        throw std::runtime_error("Erreur: une chaine de caractere doit etre composee de caracteres alphanumeriques");
    }
    Token chaine = equation[static_cast<size_t>(index)];
    index++;

    if (index >= static_cast<int>(equation.size()) || equation[static_cast<size_t>(index)].type != TOKEN_GUILLEMET) {
        throw std::runtime_error("Erreur: une chaine de caractere doit finir par un guillemet");
    }

    for (size_t caractereIndex = 0; caractereIndex < chaine.value.size(); caractereIndex++) {
        int ascii = static_cast<unsigned char>(chaine.value[caractereIndex]);
        Token token;
        token.type = TOKEN_LIT_INT;
        token.value = std::to_string(ascii);
        token.ligne = chaine.ligne;
        token.colonne = chaine.colonne;
        elementsString.push_back(new (_contexteExpression.getArena()->Allocate<NoeudLitteral>()) NoeudLitteral(token)); // NOLINT
    }

    Token tokenZero;
    tokenZero.type = TOKEN_LIT_INT;
    tokenZero.value = "0";
    tokenZero.ligne = chaine.ligne;
    tokenZero.colonne = chaine.colonne;
    elementsString.push_back(new (_contexteExpression.getArena()->Allocate<NoeudLitteral>()) NoeudLitteral(tokenZero)); // NOLINT

    return new (_contexteExpression.getArena()->Allocate<NoeudTableauInitialisation>()) NoeudTableauInitialisation(elementsString); // NOLINT
}

#endif /* EXPRESSION_STRING_CPP */