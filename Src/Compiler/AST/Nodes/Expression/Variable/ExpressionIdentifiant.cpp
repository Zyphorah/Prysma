#ifndef EXPRESSION_IDENTIFIANT_CPP
#define EXPRESSION_IDENTIFIANT_CPP

#include "Compiler/Variable/ExpressionIdentifiant.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/ContextExpression.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include <cstddef>
#include <vector>

ExpressionIdentifiant::ExpressionIdentifiant(ContextExpression& contexteExpression)
    : _contexteExpression(contexteExpression)
{}

ExpressionIdentifiant::~ExpressionIdentifiant()
= default;

auto ExpressionIdentifiant::construire(std::vector<Token>& equation) -> INode*
{
    bool isArray = false;
    size_t indexCrochet = 0;
    std::vector<Token> equationIndex;

    for (size_t index = 0; index < equation.size(); index++) {
        if (equation[index].type == TOKEN_CROCHET_OUVERT) {
            indexCrochet = index;
            isArray = true;
            break;
        }
    }

    if (isArray) {
        for (size_t index = indexCrochet; index < equation.size(); index++) {
            if (equation[index].type == TOKEN_CROCHET_FERME) {
                break;
            }
            if (index != indexCrochet) {
                equationIndex.push_back(equation[index]);
            }
        }

        INode* indexEquation = _contexteExpression.getBuilderTreeEquation()->construire(equationIndex);

        if (indexCrochet == 3 && equation[1].type == TOKEN_POINT) {
            Token nomCombine;
            nomCombine.value = equation[0].value + "." + equation[2].value;
            nomCombine.type = TOKEN_IDENTIFIANT;
            return _contexteExpression.getBuilderTreeEquation()->allouer<NodeReadingArray>(indexEquation, nomCombine);
        }

        return _contexteExpression.getBuilderTreeEquation()->allouer<NodeReadingArray>(indexEquation, equation[0]);
    }

    if (equation.size() == 3 && equation[1].type == TOKEN_POINT) {
        return _contexteExpression.getBuilderTreeEquation()->allouer<NodeAccesAttribute>(equation[0], equation[2]);
    }

    return _contexteExpression.getBuilderTreeEquation()->allouer<NodeLiteral>(equation[0]);
}

#endif /* EXPRESSION_IDENTIFIANT_CPP */