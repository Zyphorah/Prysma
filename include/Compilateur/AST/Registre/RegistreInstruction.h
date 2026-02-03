#pragma once

#include "Compilateur/AST/Registre/RegistreGeneric.h"
#include "Compilateur/Parsing/Interfaces/IParser.h"
#include <memory>
#include <set>
#include "Compilateur/Lexer/TokenType.h"

class RegistreInstruction : public RegistreGeneric<TokenType, std::shared_ptr<IParser>> {
public:
    RegistreInstruction() = default;
    virtual ~RegistreInstruction() = default;


    void enregistrer(TokenType typeToken, std::shared_ptr<IParser> fournisseur) {
        RegistreGeneric::enregistrer(typeToken, std::move(fournisseur));
    }
  
    std::shared_ptr<IParser> recuperer(TokenType typeToken) {
        return RegistreGeneric::recuperer(typeToken);
    }


    [[nodiscard]] bool estInstruction(TokenType typeToken) const {
        return existe(typeToken);
    }

    [[nodiscard]] std::set<TokenType> obtenirTypesTokens() const {
        return obtenirCles();
    }
};
