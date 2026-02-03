#pragma once

#include "Compilateur/AST/Registre/RegistreGeneric.h"
#include "Compilateur/AST/Registre/Interfaces/IRegistreSymbole.h"
#include <functional>
#include <memory>


class IExpression;

class RegistreSymbole : public RegistreGeneric<TokenType,std::function<std::shared_ptr<IExpression>()>>,
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

  
    [[nodiscard]] bool estOperateur(TokenType symbole) const override {
        return existe(symbole);
    }

    [[nodiscard]] std::set<TokenType> obtenirSymboles() const override {
        return obtenirCles();
    }

protected:
   
    [[nodiscard]] std::string genererMessageErreur(TokenType cle) const  {
        return RegistreGeneric::genererMessageErreur(cle);
    }
};