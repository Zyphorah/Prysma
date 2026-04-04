//===-- i_manager_operator.h -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#pragma once

#include "compiler/lexer/lexer.h"
#include <vector>


class IOperatorManager;

/**
 * @interface IOperatorManager
 * @brief Interface for the operator chain of responsibility
 */
class IOperatorManager {
public:
    IOperatorManager() = default;

    IOperatorManager(const IOperatorManager&) = delete;
    auto operator=(const IOperatorManager&) -> IOperatorManager& = delete;
    IOperatorManager(IOperatorManager&&) = delete;
    auto operator=(IOperatorManager&&) -> IOperatorManager& = delete;

    virtual ~IOperatorManager() = default;
    
    /**
     * @brief Sets the next link in the chain
     * @param next The next manager
     */
    virtual void setNext(IOperatorManager* next) = 0;
    
    /**
     * @brief Handles the search for an operator
     * @param equation The equation to process
     * @return The index of the found operator, or -1
     */
    virtual auto handle(const std::vector<Token>& equation) -> int = 0;
};
