#ifndef EXPRESSION_ARRAYINITIALIZATION_CPP
#define EXPRESSION_ARRAYINITIALIZATION_CPP

#include "Compiler/Array/ExpressionArrayInitialization.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/ContextExpression.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include <cstddef>
#include <vector>

ExpressionArrayInitialization::ExpressionArrayInitialization(ContextExpression& expressionContext)
    : _context(expressionContext)
{}

ExpressionArrayInitialization::~ExpressionArrayInitialization()
= default;

auto ExpressionArrayInitialization::build(std::vector<Token>& equation) -> INode*
{
    std::vector<INode*> arrayElements;
    std::vector<Token> subEquation;
    size_t index = 1;

    while (index < equation.size() && equation[index].type != TOKEN_BRACKET_CLOSE) {
        if (equation[index].type == TOKEN_COMMA) {
            index++;
            INode* element = _context.getBuilderTreeEquation()->build(subEquation);
            if (element != nullptr) {
                arrayElements.push_back(element);
            }
            subEquation.clear();
            continue;
        }

        subEquation.push_back(equation[index]);
        index++;
    }

    if (!subEquation.empty()) {
        INode* element = _context.getBuilderTreeEquation()->build(subEquation);
        if (element != nullptr) {
            arrayElements.push_back(element);
        }
    }

    return _context.getBuilderTreeEquation()->allocate<NodeArrayInitialization>(arrayElements);
}

#endif /* EXPRESSION_ARRAYINITIALIZATION_CPP */