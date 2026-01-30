#pragma once

#include "Compilateur/Parsing/Interfaces/IParser.h"
#include "Compilateur/Lexer/TokenType.h"
#include <map>
#include <memory>
#include <set>

class RegistreInstruction {
private:
    std::map<TokenType, std::shared_ptr<IParser>> _instructions;

public:
    RegistreInstruction() = default;
    virtual ~RegistreInstruction() = default;

    void enregistrer(
        TokenType typeToken,
       std::shared_ptr<IParser> fournisseur
    );

    std::shared_ptr<IParser> recuperer(TokenType typeToken);

    bool estInstruction(TokenType typeToken) const;

    std::set<TokenType> obtenirTypesTokens() const;
};
