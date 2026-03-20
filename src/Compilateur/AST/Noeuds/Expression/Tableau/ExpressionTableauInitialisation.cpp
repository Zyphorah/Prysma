#ifndef EXPRESSION_TABLEAUINITIALISATION_CPP
#define EXPRESSION_TABLEAUINITIALISATION_CPP

#include "Compilateur/Tableau/ExpressionTableauInitialisation.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Registre/ContexteExpression.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Lexer/TokenType.h"
#include <cstddef>
#include <vector>

ExpressionTableauInitialisation::ExpressionTableauInitialisation(ContexteExpression& contexteExpression)
    : _contexteExpression(contexteExpression)
{}

ExpressionTableauInitialisation::~ExpressionTableauInitialisation()
= default;

auto ExpressionTableauInitialisation::construire(std::vector<Token>& equation) -> INoeud*
{
    std::vector<INoeud*> elementsTableau;
    std::vector<Token> sousEquation;
    size_t index = 1;

    while (index < equation.size() && equation[index].type != TOKEN_CROCHET_FERME) {
        if (equation[index].type == TOKEN_VIRGULE) {
            index++;
            INoeud* element = _contexteExpression.getConstructeurArbreEquation()->construire(sousEquation);
            if (element != nullptr) {
                elementsTableau.push_back(element);
            }
            sousEquation.clear();
            continue;
        }

        sousEquation.push_back(equation[index]);
        index++;
    }

    if (!sousEquation.empty()) {
        INoeud* element = _contexteExpression.getConstructeurArbreEquation()->construire(sousEquation);
        if (element != nullptr) {
            elementsTableau.push_back(element);
        }
    }

    return _contexteExpression.getConstructeurArbreEquation()->allouer<NoeudTableauInitialisation>(elementsTableau);
}

#endif /* EXPRESSION_TABLEAUINITIALISATION_CPP */