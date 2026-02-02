#pragma once

#include "Compilateur/AST/Registre/RegistreGeneric.h"
#include "Compilateur/AST/Registre/Interfaces/IRegistreSymbole.h"
#include <functional>
#include <memory>


class IExpression;

class RegistreSymbole : public RegistreGeneric<std::function<std::shared_ptr<IExpression>()>>,
                        public IRegistreSymbole {
public:
    RegistreSymbole() = default;
    virtual ~RegistreSymbole() = default;

  
    void enregistrer(
        TokenType symbole, 
        std::function<std::shared_ptr<IExpression>()> fournisseur) override {
        RegistreGeneric::enregistrer(symbole, std::move(fournisseur));
    }


    std::shared_ptr<IExpression> recupererNoeud(TokenType symbole) override {
        auto fournisseur = RegistreGeneric::recuperer(symbole);
        return fournisseur();
    }

  
    bool estOperateur(TokenType symbole) const override {
        return existe(symbole);
    }

    std::set<TokenType> obtenirSymboles() const override {
        return obtenirCles();
    }

protected:
   
    std::string genererMessageErreur(TokenType cle) const override {
        if (_messageErreurCallback) {
            return _messageErreurCallback(cle);
        }
        return std::string("Symbole inconnu: ") + std::to_string(cle);
    }
};