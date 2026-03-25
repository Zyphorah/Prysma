#ifndef EXPRESSION_TABLEAUINITIALISATION_CPP
#define EXPRESSION_TABLEAUINITIALISATION_CPP

#include "Compiler/Array/ExpressionArrayInitialization.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/ContextExpression.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include <cstddef>
#include <vector>

ExpressionArrayInitialization::ExpressionArrayInitialization(ContextExpression& contexteExpression)
    : _contexteExpression(contexteExpression)
{}

ExpressionArrayInitialization::~ExpressionArrayInitialization()
= default;

auto ExpressionArrayInitialization::construire(std::vector<Token>& equation) -> INode*
{
    std::vector<INode*> elementsArray;
    std::vector<Token> sousEquation;
    size_t index = 1;

    while (index < equation.size() && equation[index].type != TOKEN_CROCHET_FERME) {
        if (equation[index].type == TOKEN_VIRGULE) {
            index++;
            INode* element = _contexteExpression.getBuilderTreeEquation()->construire(sousEquation);
            if (element != nullptr) {
                elementsArray.push_back(element);
            }
            sousEquation.clear();
            continue;
        }

        sousEquation.push_back(equation[index]);
        index++;
    }

    if (!sousEquation.empty()) {
        INode* element = _contexteExpression.getBuilderTreeEquation()->construire(sousEquation);
        if (element != nullptr) {
            elementsArray.push_back(element);
        }
    }

    return _contexteExpression.getBuilderTreeEquation()->allouer<NodeArrayInitialization>(elementsArray);
}

#endif /* EXPRESSION_TABLEAUINITIALISATION_CPP */