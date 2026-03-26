#pragma once

#include "Compiler/AST/Registry/RegistryGeneric.h"
#include "Compiler/AST/Registry/Interfaces/IRegistrySymbole.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include <functional>
#include <set>
#include <string>
#include <utility>


class IExpression;

class RegistrySymbol : public RegistryGeneric<TokenType, std::function<IExpression*(Token)>>,
                      public IRegistrySymbol {
public:
    RegistrySymbol() = default;
    RegistrySymbol(const RegistrySymbol&) = delete;
    auto operator=(const RegistrySymbol&) -> RegistrySymbol& = delete;
    RegistrySymbol(RegistrySymbol&&) = delete;
    auto operator=(RegistrySymbol&&) -> RegistrySymbol& = delete;
    ~RegistrySymbol() override = default;

    void registerSymbol(
        TokenType symbol, 
        std::function<IExpression*(Token)> provider) override {
        RegistryGeneric::registerElement(symbol, std::move(provider));
    }

    auto getNode(Token token) -> IExpression* override {
        std::function<IExpression*(Token)> provider = RegistryGeneric::get(token.type);
        return provider(token);
    }

    [[nodiscard]] auto isOperator(TokenType symbol) const -> bool override {
        return exists(symbol);
    }

    [[nodiscard]] auto getSymbols() const -> std::set<TokenType> override {
        return getKeys();
    }

protected:
    using RegistryGeneric<TokenType, std::function<IExpression*(Token)>>::generateErrorMessage;
    [[nodiscard]] auto generateErrorMessage(const TokenType& key) const -> std::string override {
        return RegistryGeneric::generateErrorMessage(key);
    }
};