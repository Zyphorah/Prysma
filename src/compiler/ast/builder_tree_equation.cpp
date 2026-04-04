//===-- builder_tree_equation.cpp -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#include "compiler/ast/builder_tree_equation.h"
#include <cstddef>
#include <llvm/Support/Allocator.h>
#include <vector>
#include "compiler/ast/nodes/interfaces/i_expression.h"
#include "compiler/ast/nodes/interfaces/i_node.h"
#include "compiler/ast/registry/registry_expression.h"
#include "compiler/ast/registry/registry_symbole.h"
#include "compiler/parser/equation/chain_of_responsibility.h"
#include "compiler/parser/equation/interfaces/i_manager_parenthesis.h"
#include "compiler/manager_error.h"
#include "compiler/lexer/lexer.h"
#include "compiler/lexer/token_type.h"

BuilderTreeEquation::BuilderTreeEquation(
    ChainOfResponsibility* chainOfResponsibility,
    RegistrySymbol* symbolRegistry,
    RegistryExpression* expressionRegistry,
    IManagerParenthesis* parenthesisManager,
    llvm::BumpPtrAllocator& arena)
    : _chainOfResponsibility(chainOfResponsibility), 
      _symbolRegistry(symbolRegistry), 
      _expressionRegistry(expressionRegistry),
      _parenthesisManager(parenthesisManager),
      _arena(arena),
      _lastToken{TOKEN_EOF, "", 1, 1}
{
}

auto BuilderTreeEquation::build(std::vector<Token> &tokens) -> INode* {
    std::vector<Token> tokensWithoutParentheses = _parenthesisManager->removeWrappingParentheses(tokens);
    tokens = tokensWithoutParentheses;
    
    if (tokens.empty()) {
        throw CompilationError("Error: empty equation", Line(_lastToken.line), Column(_lastToken.column));
    }
    
    int index = _chainOfResponsibility->findOperator(tokens);

    if (index == -1) {
        TokenType type = tokens[0].type;

        if ((_expressionRegistry != nullptr) && _expressionRegistry->exists(type)) {
            return _expressionRegistry->get(type)->build(tokens);
        }

        throw CompilationError("Error: unrecognized token in the equation", Line(tokens[0].line), Column(tokens[0].column));
    }
    
    IExpression* node = _symbolRegistry->getNode(tokens[static_cast<size_t>(index)]);
    std::vector<Token> left(tokens.begin(), tokens.begin() + index); 
    std::vector<Token> right(tokens.begin() + index + 1, tokens.end());
    
    INode* leftExpr = build(left);
    INode* rightExpr = build(right);
    
    node->addExpression(leftExpr, rightExpr);
    return node;
}

auto BuilderTreeEquation::build(std::vector<Token>& tokens, int& index) -> INode* {

    // Save the current token position for error messages
    if (index < static_cast<int>(tokens.size())) {
        _lastToken = tokens[static_cast<size_t>(index)];
    }

    // Level system to calculate depth, mandatory to avoid issues with separation like 34+4)) otherwise the 
    // system doesn't know what to do with the two remaining parentheses. 
    
    std::vector<Token> equationTokens;
    int parenthesisDepth = 0;
    int bracketDepth = 0;

    while(index < static_cast<int>(tokens.size())) {
        TokenType type = tokens[static_cast<size_t>(index)].type;

        if (type == TOKEN_PAREN_OPEN) {
            parenthesisDepth++;
        }
        else if (type == TOKEN_PAREN_CLOSE) {
            if (parenthesisDepth == 0) {
                break; 
            }
            parenthesisDepth--;
        }
        else if (type == TOKEN_BRACKET_OPEN) {
            bracketDepth++;
        }
        else if (type == TOKEN_BRACKET_CLOSE) {
            if (bracketDepth == 0) {
                break;
            }
            bracketDepth--;
        }
        
        if (parenthesisDepth == 0 && bracketDepth == 0) {
            if (type == TOKEN_SEMICOLON || 
                type == TOKEN_BRACE_CLOSE || 
                type == TOKEN_COMMA ) {
                break;
            }
        }

        equationTokens.push_back(tokens[static_cast<size_t>(index)]);
        index++;
    }

    return build(equationTokens);
}

llvm::BumpPtrAllocator& BuilderTreeEquation::getArena()
{
    return _arena;
}
