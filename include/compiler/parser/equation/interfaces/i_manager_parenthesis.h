//===-- i_manager_parenthesis.h -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#pragma once

#include "compiler/lexer/lexer.h"
#include <vector>

/**
 * @interface IManagerParenthesis
 * @brief Manages detection and removal of parentheses
 */
class IManagerParenthesis {
public:
    IManagerParenthesis() = default;
    virtual ~IManagerParenthesis() = default;
    IManagerParenthesis(const IManagerParenthesis&) = delete;
    auto operator=(const IManagerParenthesis&) -> IManagerParenthesis& = delete;
    IManagerParenthesis(IManagerParenthesis&&) = delete;
    auto operator=(IManagerParenthesis&&) -> IManagerParenthesis& = delete;
    
    /**
     * @brief Removes parentheses that wrap the entire expression
     * Ex: "(1+2)" -> "1+2", "(1+2)*(3+4)" remains unchanged
     * @param equation The equation to process
     * @return The equation without wrapping parentheses
     */
    virtual auto removeWrappingParentheses(const std::vector<Token>& equation) -> std::vector<Token> = 0;
    
    /**
     * @brief Finds the last operator at parenthesis level zero
     * @param equation The equation to analyze
     * @param op The operator token to search for
     * @return The index of the operator, or -1 if not found
     */
    virtual auto findLastAtLevelZero(const std::vector<Token>& equation, Token op) -> int = 0;
};
