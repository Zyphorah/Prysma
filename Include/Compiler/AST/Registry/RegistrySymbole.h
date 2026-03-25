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

class RegistrySymbole : public RegistryGeneric<TokenType,std::function<IExpression*(Token)>>,
                        public IRegistrySymbole {
public:
    RegistrySymbole() = default;
    RegistrySymbole(const RegistrySymbole&) = delete;
    auto operator=(const RegistrySymbole&) -> RegistrySymbole& = delete;
    RegistrySymbole(RegistrySymbole&&) = delete;
    auto operator=(RegistrySymbole&&) -> RegistrySymbole& = delete;
    ~RegistrySymbole() override = default;

  
    void enregistryr(
        TokenType symbole, 
        std::function<IExpression*(Token)> fournisseur) override {
        RegistryGeneric::enregistryr(symbole, std::move(fournisseur));
    }


    auto recupererNode(Token token) -> IExpression* override {
        std::function<IExpression*(Token)> fournisseur = RegistryGeneric::recuperer(token.type);
        return fournisseur(token);
    }

  
    [[nodiscard]] auto estOperateur(TokenType symbole) const -> bool override {
        return existe(symbole);
    }

    [[nodiscard]] auto obtenirSymboles() const -> std::set<TokenType> override {
        return obtenirCles();
    }

protected:
   
    using RegistryGeneric<TokenType, std::function<IExpression*(Token)>>::generatedrMessageError;
    [[nodiscard]] auto generatedrMessageError(const TokenType& cle) const -> std::string override {
        return RegistryGeneric::generatedrMessageError(cle);
    }
};