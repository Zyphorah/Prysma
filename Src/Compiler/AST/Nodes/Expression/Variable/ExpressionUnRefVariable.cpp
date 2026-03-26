#ifndef EXPRESSION_UNREFVARIABLE_CPP
#define EXPRESSION_UNREFVARIABLE_CPP

#include "Compiler/Variable/ExpressionUnRefVariable.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/ContextExpression.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include <stdexcept>
#include <vector>

ExpressionUnRefVariable::ExpressionUnRefVariable(ContextExpression& expressionContext)
    : _context(expressionContext)
{}

ExpressionUnRefVariable::~ExpressionUnRefVariable()
= default;

auto ExpressionUnRefVariable::build(std::vector<Token>& equation) -> INode*
{
    if (equation.size() < 2 || equation[1].type != TOKEN_IDENTIFIER) {
        throw std::runtime_error("Error: 'unref' must be followed by an identifier");
    }
    return _context.getBuilderTreeEquation()->allocate<NodeUnRefVariable>(equation[1].value); 
}

#endif /* EXPRESSION_UNREFVARIABLE_CPP */