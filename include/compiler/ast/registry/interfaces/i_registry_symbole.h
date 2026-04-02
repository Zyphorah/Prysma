#pragma once

#include <set>
#include <functional>
#include "compiler/macros/prysma_nodiscard.h"
#include "compiler/lexer/token_type.h"
#include "compiler/lexer/lexer.h"

class IExpression;

/**
 * @interface IRegistrySymbol
 * @brief Registry of mathematical operators with priority management
 * The registration order determines the priority:
 * - Registered FIRST: LOWER priority
 * - Registered LAST: HIGHER priority
 */
class IRegistrySymbol {
public:
    IRegistrySymbol() = default;

    virtual ~IRegistrySymbol() = default;
    IRegistrySymbol(const IRegistrySymbol&) = delete;
    auto operator=(const IRegistrySymbol&) -> IRegistrySymbol& = delete;
    IRegistrySymbol(IRegistrySymbol&&) = delete;
    auto operator=(IRegistrySymbol&&) -> IRegistrySymbol& = delete;

    virtual void registerSymbol(TokenType symbol, std::function<IExpression*(Token)> provider) = 0;
    virtual auto getNode(Token token) -> IExpression* = 0;
    
    /**
     * @brief Checks if a character is a known operator
     * @param symbol The character to check
     * @return true if it is a known operator
     */
    PRYSMA_NODISCARD virtual auto isOperator(TokenType symbol) const -> bool = 0;
    
    /**
     * @brief Gets the set of registered symbols
     * @return Set of operator symbols
     */
    PRYSMA_NODISCARD virtual auto getSymbols() const -> std::set<TokenType> = 0;
};
