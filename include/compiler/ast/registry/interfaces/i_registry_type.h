//===-- i_registry_type.h ---------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#ifndef DD937933_DED1_40A8_819C_360C96A8F41C
#define DD937933_DED1_40A8_819C_360C96A8F41C

#include "compiler/ast/registry/registry_generic.h"
#include "compiler/lexer/token_type.h"
#include "compiler/ast/registry/types/i_type.h"

class IRegistryType : public RegistryGeneric<TokenType, IType*> {
public:
    IRegistryType() = default;
    IRegistryType(const IRegistryType&) = delete;
    auto operator=(const IRegistryType&) -> IRegistryType& = delete;
    IRegistryType(IRegistryType&&) = delete;
    auto operator=(IRegistryType&&) -> IRegistryType& = delete;
     ~IRegistryType() override = default;
};

#endif /* DD937933_DED1_40A8_819C_360C96A8F41C */
