#pragma once

#include "Compilateur/AST/Registre/RegistreGeneric.h"
#include "Compilateur/Parsing/Interfaces/IParser.h"
#include <memory>


class RegistreInstruction : public RegistreGeneric<std::shared_ptr<IParser>> {
public:
    RegistreInstruction() = default;
    virtual ~RegistreInstruction() = default;

 
    void enregistrer(TokenType typeToken, std::shared_ptr<IParser> fournisseur) {
        RegistreGeneric::enregistrer(typeToken, std::move(fournisseur));
    }

  
    std::shared_ptr<IParser> recuperer(TokenType typeToken) {
        return RegistreGeneric::recuperer(typeToken);
    }


    bool estInstruction(TokenType typeToken) const {
        return existe(typeToken);
    }

    std::set<TokenType> obtenirTypesTokens() const {
        return obtenirCles();
    }

protected:
   
    std::string genererMessageErreur(TokenType cle) const override {
        if (_messageErreurCallback) {
            return _messageErreurCallback(cle);
        }
        return std::string("Type de token inconnu: ") + std::to_string(cle);
    }
};
