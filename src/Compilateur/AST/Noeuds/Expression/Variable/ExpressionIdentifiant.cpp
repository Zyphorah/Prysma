#ifndef EXPRESSION_IDENTIFIANT_CPP
#define EXPRESSION_IDENTIFIANT_CPP

#include "Compilateur/Variable/ExpressionIdentifiant.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/Lexer/TokenType.h"
#include <vector>

ExpressionIdentifiant::ExpressionIdentifiant(ContexteExpression& contexteExpression)
    : _contexteExpression(contexteExpression)
{}

ExpressionIdentifiant::~ExpressionIdentifiant()
{}

INoeud* ExpressionIdentifiant::construire(std::vector<Token>& equation)
{
    bool isTableau = false;
    size_t indexCrochet = 0;
    std::vector<Token> equationIndex;

    for (size_t index = 0; index < equation.size(); index++) {
        if (equation[index].type == TOKEN_CROCHET_OUVERT) {
            indexCrochet = index;
            isTableau = true;
            break;
        }
    }

    if (isTableau) {
        for (size_t index = indexCrochet; index < equation.size(); index++) {
            if (equation[index].type == TOKEN_CROCHET_FERME) {
                break;
            }
            if (index != indexCrochet) {
                equationIndex.push_back(equation[index]);
            }
        }

        INoeud* indexEquation = _contexteExpression.constructeurArbreEquation->construire(equationIndex);
        return _contexteExpression.constructeurArbreEquation->allouer<NoeudLectureTableau>(indexEquation, equation[0]);
    }

    if (equation.size() == 3 && equation[1].type == TOKEN_POINT) {
        return _contexteExpression.constructeurArbreEquation->allouer<NoeudAccesAttribut>(equation[0], equation[2]);
    }

    return _contexteExpression.constructeurArbreEquation->allouer<NoeudLitteral>(equation[0]);
}

#endif /* EXPRESSION_IDENTIFIANT_CPP */