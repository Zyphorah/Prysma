#ifndef EXPRESSION_REFVARIABLE_CPP
#define EXPRESSION_REFVARIABLE_CPP

#include "Compiler/Variable/ExpressionRefVariable.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/ContextExpression.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include <stdexcept>
#include <vector>

ExpressionRefVariable::ExpressionRefVariable(ContextExpression& contexteExpression)
    : _contexteExpression(contexteExpression)
{}

ExpressionRefVariable::~ExpressionRefVariable()
= default;

auto ExpressionRefVariable::construire(std::vector<Token>& equation) -> INode*
{
    if (equation.size() < 2 || equation[1].type != TOKEN_IDENTIFIANT) {
        throw std::runtime_error("Error: 'ref' doit être suivi d'un identifiant");
    }
    return _contexteExpression.getBuilderTreeEquation()->allouer<NodeRefVariable>(equation[1].value); 
}

#endif /* EXPRESSION_REFVARIABLE_CPP */