//===-- expression_identifiant.cpp ------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#ifndef EXPRESSION_IDENTIFIER_CPP
#define EXPRESSION_IDENTIFIER_CPP

#include "compiler/variable/expression_identifiant.h"
#include "compiler/ast/ast_genere.h"
#include "compiler/ast/nodes/interfaces/i_node.h"
#include "compiler/ast/registry/context_expression.h"
#include "compiler/ast/type_resolut.h"
#include "compiler/lexer/lexer.h"
#include "compiler/lexer/token_type.h"
#include <cstddef>
#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/Support/FormatVariadic.h>
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
            std::string tempStr = llvm::formatv("{0}.{1}", equation[0].value, equation[2].value).str();
            
            auto& arena = _context.getBuilderTreeEquation()->getArena();
            char* arr = static_cast<char*>(arena.Allocate(tempStr.size() + 1, 1));
            std::memcpy(arr, tempStr.c_str(), tempStr.size() + 1);

            combinedName.value = llvm::StringRef(arr, tempStr.size());
            combinedName.type = TOKEN_IDENTIFIER;
            return _context.getBuilderTreeEquation()->allocate<NodeReadingArray>(indexExpr, combinedName);
        }

        return _context.getBuilderTreeEquation()->allocate<NodeReadingArray>(indexExpr, equation[0]);
    }

    // Case: object.method() or object.method(arg1, arg2)
    if (equation.size() >= 5 && equation[1].type == TOKEN_DOT &&
        equation[3].type == TOKEN_PAREN_OPEN && equation.back().type == TOKEN_PAREN_CLOSE) {
        
        INode* receiver = _context.getBuilderTreeEquation()->allocate<NodeRefVariable>(
            _context.getBuilderTreeEquation()->allocate<TypeResolut>(), 
            equation[0]);
        
        // Parse arguments between '(' and ')'
        llvm::SmallVector<INode*, 8> children;
        std::vector<Token> argTokens;
        int depth = 0;
        
        for (size_t i = 4; i < equation.size() - 1; ++i) {
            Token t = equation[i];
            if (t.type == TOKEN_PAREN_OPEN) depth++;
            else if (t.type == TOKEN_PAREN_CLOSE) depth--;
            
            if (t.type == TOKEN_COMMA && depth == 0) {
                if (!argTokens.empty()) {
                    children.push_back(_context.getBuilderTreeEquation()->build(argTokens));
                    argTokens.clear();
                }
            } else {
                argTokens.push_back(t);
            }
        }
        if (!argTokens.empty()) {
            children.push_back(_context.getBuilderTreeEquation()->build(argTokens));
        }
        
        return _context.getBuilderTreeEquation()->allocate<NodeCallObject>(
            equation[2], 
            _context.getBuilderTreeEquation()->allocate<TypeResolut>(), 
            receiver, 
            _context.getBuilderTreeEquation()->allocateArray<INode*>(children)
        );
    }

    if (equation.size() == 3 && equation[1].type == TOKEN_DOT) {
        return _context.getBuilderTreeEquation()->allocate<NodeAccesAttribute>(equation[0], equation[2]);
    }

    return _context.getBuilderTreeEquation()->allocate<NodeLiteral>(equation[0]);
}

#endif /* EXPRESSION_IDENTIFIER_CPP */
