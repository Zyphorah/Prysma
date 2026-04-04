//===-- i_expression.h -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#pragma once

#include "i_node.h"

/**
 * @interface IExpression
 * @brief Interface for nodes of an expression tree
 * Inherits from INode
 */
class IExpression : public INode {
public:
    IExpression() = default;
    ~IExpression() override = default;

    IExpression(const IExpression&) = delete;
    auto operator=(const IExpression&) -> IExpression& = delete;
    IExpression(IExpression&&) = delete;
    auto operator=(IExpression&&) -> IExpression& = delete;
    
    /**
     * @brief Adds two expressions as left and right children
     * @param left Left child expression
     * @param right Right child expression
     * @return Reference to the current node
     */
    virtual auto addExpression(
        INode* left, 
        INode* right
    ) -> IExpression* = 0;
};
