//===-- registry_instruction.h -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#pragma once

#include "compiler/ast/registry/registry_generic.h"
#include "compiler/parser/interfaces/i_parser.h"
#include "compiler/lexer/token_type.h"

class RegistryInstruction : public RegistryGeneric<TokenType, IParser*> {
public:
    RegistryInstruction() = default;
    ~RegistryInstruction() override = default;

    RegistryInstruction(const RegistryInstruction&) = delete;
    auto operator=(const RegistryInstruction&) -> RegistryInstruction& = delete;
    RegistryInstruction(RegistryInstruction&&) = delete;
    auto operator=(RegistryInstruction&&) -> RegistryInstruction& = delete;
};
