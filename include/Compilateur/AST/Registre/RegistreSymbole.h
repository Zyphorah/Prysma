#pragma once

#include "Compilateur/AST/Registre/RegistreGeneric.h"
#include "Compilateur/AST/Registre/Interfaces/IRegistreSymbole.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Lexer/TokenType.h"
#include <functional>
#include <set>
#include <string>
#include <utility>


class IExpression;

class RegistreSymbole : public RegistreGeneric<TokenType,std::function<IExpression*(Token)>>,
                        public IRegistreSymbole {
public:
    RegistreSymbole() = default;
    RegistreSymbole(const RegistreSymbole&) = delete;
    auto operator=(const RegistreSymbole&) -> RegistreSymbole& = delete;
    RegistreSymbole(RegistreSymbole&&) = delete;
    auto operator=(RegistreSymbole&&) -> RegistreSymbole& = delete;
    ~RegistreSymbole() override = default;

  
    void enregistrer(
        TokenType symbole, 
        std::function<IExpression*(Token)> fournisseur) override {
        RegistreGeneric::enregistrer(symbole, std::move(fournisseur));
    }


    auto recupererNoeud(Token token) -> IExpression* override {
        std::function<IExpression*(Token)> fournisseur = RegistreGeneric::recuperer(token.type);
        return fournisseur(token);
    }

  
    [[nodiscard]] auto estOperateur(TokenType symbole) const -> bool override {
        return existe(symbole);
    }

    [[nodiscard]] auto obtenirSymboles() const -> std::set<TokenType> override {
        return obtenirCles();
    }

protected:
   
    using RegistreGeneric<TokenType, std::function<IExpression*(Token)>>::genererMessageErreur;
    [[nodiscard]] auto genererMessageErreur(const TokenType& cle) const -> std::string override {
        return RegistreGeneric::genererMessageErreur(cle);
    }
};