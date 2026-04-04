//===-- parser_base.cpp -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#include "compiler/parser/parser_base.h"
#include "compiler/ast/interfaces/i_builder_tree.h"
#include "compiler/ast/nodes/interfaces/i_node.h"
#include "compiler/lexer/lexer.h"
#include "compiler/lexer/token_type.h"
#include "compiler/manager_error.h"
#include <llvm/ADT/SmallVector.h>
#include <cstddef>
#include <string>
#include <vector>

namespace {
    constexpr int DEFAULT_CHILDREN_CAPACITY = 32;
}

auto ParserBase::consume(std::vector<Token>& tokens, int& index, TokenType expectedType, const std::string& errorMessage) -> Token
{
    if (index < 0 || index >= static_cast<int>(tokens.size()) || tokens[static_cast<size_t>(index)].type != expectedType) {
        if (index >= 0 && index < static_cast<int>(tokens.size())) {
            throw CompilationError(errorMessage, Line(tokens[static_cast<size_t>(index)].line), Column(tokens[static_cast<size_t>(index)].column));
        }
        throw CompilationError(errorMessage, Line(1), Column(1));
    }
    return tokens[static_cast<size_t>(index++)];
}

auto ParserBase::consumeChildBody(std::vector<Token>& tokens, int& index , IBuilderTree* builderTree, TokenType end) -> llvm::ArrayRef<INode*>
{
    llvm::SmallVector<INode*, DEFAULT_CHILDREN_CAPACITY> children;
    while(index < static_cast<int>(tokens.size()) && tokens[static_cast<size_t>(index)].type != end)
    {
        // Ignore commas between elements
        if(tokens[static_cast<size_t>(index)].type == TOKEN_COMMA)
        {
            index++;
            continue;
        }
        
        INode* child = builderTree->build(tokens, index);
        children.push_back(child);
    }
    return builderTree->allocateArray<INode*>(children);
}
