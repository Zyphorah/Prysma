#ifndef EXPRESSION_IDENTIFIER_CPP
#define EXPRESSION_IDENTIFIER_CPP

#include "compiler/variable/expression_identifiant.h"
#include "compiler/ast/ast_genere.h"
#include "compiler/ast/nodes/interfaces/i_node.h"
#include "compiler/ast/registry/context_expression.h"
#include "compiler/lexer/lexer.h"
#include "compiler/lexer/token_type.h"
#include <cstddef>
#include <llvm/ADT/StringRef.h>
#include <string.h>
#include <string>
#include <vector>
#include <cstring>

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
            std::string tempStr = equation[0].value.str() + "." + equation[2].value.str();
            
            auto& arena = _context.getBuilderTreeEquation()->getArena();
            char* arr = static_cast<char*>(arena.Allocate(tempStr.size() + 1, 1));
            std::memcpy(arr, tempStr.c_str(), tempStr.size() + 1);

            combinedName.value = llvm::StringRef(arr, tempStr.size());
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