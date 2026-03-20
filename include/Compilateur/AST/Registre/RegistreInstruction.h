#pragma once

#include "Compilateur/AST/Registre/RegistreGeneric.h"
#include "Compilateur/AnalyseSyntaxique/Interfaces/IParseur.h"
#include "Compilateur/Lexer/TokenType.h"

class RegistreInstruction : public RegistreGeneric<TokenType, IParseur*> {
public:
    RegistreInstruction() = default;
    ~RegistreInstruction() override = default;

    RegistreInstruction(const RegistreInstruction&) = delete;
    auto operator=(const RegistreInstruction&) -> RegistreInstruction& = delete;
    RegistreInstruction(RegistreInstruction&&) = delete;
    auto operator=(RegistreInstruction&&) -> RegistreInstruction& = delete;
};
