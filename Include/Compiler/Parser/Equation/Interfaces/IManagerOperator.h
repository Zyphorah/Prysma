#pragma once

#include "Compiler/Lexer/Lexer.h"
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
