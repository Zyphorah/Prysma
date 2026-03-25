#pragma once

#include "Compiler/AST/Registry/RegistryGeneric.h"
#include "Compiler/Parser/Interfaces/IParser.h"
#include "Compiler/Lexer/TokenType.h"

class RegistryInstruction : public RegistryGeneric<TokenType, IParser*> {
public:
    RegistryInstruction() = default;
    ~RegistryInstruction() override = default;

    RegistryInstruction(const RegistryInstruction&) = delete;
    auto operator=(const RegistryInstruction&) -> RegistryInstruction& = delete;
    RegistryInstruction(RegistryInstruction&&) = delete;
    auto operator=(RegistryInstruction&&) -> RegistryInstruction& = delete;
};
