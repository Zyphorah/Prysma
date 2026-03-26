#pragma once

#include "INode.h"

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
