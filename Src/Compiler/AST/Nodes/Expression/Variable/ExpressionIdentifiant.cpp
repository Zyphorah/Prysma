#ifndef EXPRESSION_IDENTIFIER_CPP
#define EXPRESSION_IDENTIFIER_CPP

#include "Compiler/Variable/ExpressionIdentifiant.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/ContextExpression.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include <cstddef>
#include <vector>

ExpressionIdentifiant::ExpressionIdentifiant(ContextExpression& expressionContext)
    : _context(expressionContext)
{}

ExpressionIdentifiant::~ExpressionIdentifiant()
= default;

auto ExpressionIdentifiant::build(std::vector<Token>& equation) -> INode*
{
    bool isArray = false;
    size_t bracketIndex = 0;
    std::vector<Token> indexEquation;

    for (size_t index = 0; index < equation.size(); index++) {
        if (equation[index].type == TOKEN_BRACKET_OPEN) {
            bracketIndex = index;
            isArray = true;
            break;
        }
    }

    if (isArray) {
        for (size_t index = bracketIndex; index < equation.size(); index++) {
            if (equation[index].type == TOKEN_BRACKET_CLOSE) {
                break;
            }
            if (index != bracketIndex) {
                indexEquation.push_back(equation[index]);
            }
        }

        INode* indexExpr = _context.getBuilderTreeEquation()->build(indexEquation);

        if (bracketIndex == 3 && equation[1].type == TOKEN_DOT) {
            Token combinedName;
            combinedName.value = equation[0].value + "." + equation[2].value;
            combinedName.type = TOKEN_IDENTIFIER;
            return _context.getBuilderTreeEquation()->allocate<NodeReadingArray>(indexExpr, combinedName);
        }

        return _context.getBuilderTreeEquation()->allocate<NodeReadingArray>(indexExpr, equation[0]);
    }

    if (equation.size() == 3 && equation[1].type == TOKEN_DOT) {
        return _context.getBuilderTreeEquation()->allocate<NodeAccesAttribute>(equation[0], equation[2]);
    }

    return _context.getBuilderTreeEquation()->allocate<NodeLiteral>(equation[0]);
}

#endif /* EXPRESSION_IDENTIFIER_CPP */