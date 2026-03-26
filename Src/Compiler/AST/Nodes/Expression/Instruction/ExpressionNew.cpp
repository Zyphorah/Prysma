#ifndef EXPRESSION_NEW_CPP
#define EXPRESSION_NEW_CPP

#include "Compiler/Instruction/ExpressionNew.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/ContextExpression.h"
#include "Compiler/ManagerError.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include <cstddef>
#include <vector>

ExpressionNew::ExpressionNew(ContextExpression& expressionContext)
    : _context(expressionContext)
{}

ExpressionNew::~ExpressionNew()
= default;

auto ExpressionNew::build(std::vector<Token>& equation) -> INode*
{
    int index = 0;
    if (equation.empty() || equation[0].type != TOKEN_NEW) {
        throw CompilationError("Error: the 'new' token is expected", Line(0), Column(0));
    }
    index++;

    if (index >= static_cast<int>(equation.size())) {
        throw CompilationError("Error: no valid type for the object created with 'new'", Line(equation[0].line), Column(equation[0].column));
    }

    Token typeName = equation[static_cast<size_t>(index)];

    if (typeName.type != TOKEN_TYPE_INT32 &&
        typeName.type != TOKEN_TYPE_INT64 &&
        typeName.type != TOKEN_TYPE_FLOAT &&
        typeName.type != TOKEN_TYPE_BOOL &&
        typeName.type != TOKEN_TYPE_CHAR &&
        typeName.type != TOKEN_TYPE_STRING &&
        typeName.type != TOKEN_TYPE_VOID &&
        typeName.type != TOKEN_TYPE_PTR &&
        typeName.type != TOKEN_IDENTIFIER) {
        throw CompilationError("Error: no valid type for the object created with 'new'", Line(typeName.line), Column(typeName.column));
    }

    // We need to fill the arguments of new (e.g., new MyClass(arg1, arg2)) by adding the childs of nodeNew
    std::vector<INode*> arguments;
    if (typeName.type == TOKEN_IDENTIFIER) {
        index++; // Skip the type name
        
        if (index < static_cast<int>(equation.size()) && equation[static_cast<size_t>(index)].type == TOKEN_PAREN_OPEN) {
            index++; // Skip '('

            while(index < static_cast<int>(equation.size()) && equation[static_cast<size_t>(index)].type != TOKEN_PAREN_CLOSE) {
                arguments.push_back(_context.getContextParser()->getBuilderTreeEquation()->build(equation, index));

                if (index < static_cast<int>(equation.size()) && equation[static_cast<size_t>(index)].type == TOKEN_COMMA) {
                    index++; // Skip the comma
                }
            }
            if (index < static_cast<int>(equation.size()) && equation[static_cast<size_t>(index)].type == TOKEN_PAREN_CLOSE) {
                index++; // Skip ')'
            }
        }
    }

    return _context.getBuilderTreeEquation()->allocate<NodeNew>(arguments, typeName);
}

#endif /* EXPRESSION_NEW_CPP */