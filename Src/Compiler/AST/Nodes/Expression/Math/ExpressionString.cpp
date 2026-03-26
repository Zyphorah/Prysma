#ifndef EXPRESSION_STRING_CPP
#define EXPRESSION_STRING_CPP

#include "Compiler/Math/ExpressionString.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/ContextExpression.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include <cstddef>
#include <stdexcept>
#include <string>
#include <vector>

ExpressionString::ExpressionString(ContextExpression& expressionContext)
    : _context(expressionContext)
{}

ExpressionString::~ExpressionString()
= default;

auto ExpressionString::build(std::vector<Token>& equation) -> INode*
{
    std::vector<INode*> stringElements;

    int index = 0;
    if (equation.empty() || equation[0].type != TOKEN_QUOTE) {
        throw std::runtime_error("Error: a string must start with a quote");
    }
    index++;

    if (index >= static_cast<int>(equation.size()) || equation[static_cast<size_t>(index)].type != TOKEN_IDENTIFIER) {
        throw std::runtime_error("Error: a string must be composed of alphanumeric characters");
    }
    Token str = equation[static_cast<size_t>(index)];
    index++;

    if (index >= static_cast<int>(equation.size()) || equation[static_cast<size_t>(index)].type != TOKEN_QUOTE) {
        throw std::runtime_error("Error: a string must end with a quote");
    }

    for (size_t charIndex = 0; charIndex < str.value.size(); charIndex++) {
        int ascii = static_cast<unsigned char>(str.value[charIndex]);
        Token token;
        token.type = TOKEN_LIT_INT;
        token.value = std::to_string(ascii);
        token.line = str.line;
        token.column = str.column;
        stringElements.push_back(_context.getBuilderTreeEquation()->allocate<NodeLiteral>(token)); 
    }

    Token tokenZero;
    tokenZero.type = TOKEN_LIT_INT;
    tokenZero.value = "0";
    tokenZero.line = str.line;
    tokenZero.column = str.column;
    stringElements.push_back(_context.getBuilderTreeEquation()->allocate<NodeLiteral>(tokenZero)); 

    return _context.getBuilderTreeEquation()->allocate<NodeArrayInitialization>(stringElements);
}

#endif /* EXPRESSION_STRING_CPP */