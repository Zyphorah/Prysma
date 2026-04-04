//===-- registry_expression.h -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#ifndef F5F52A3C_374A_47C1_868F_9E81A1FED8AD
#define F5F52A3C_374A_47C1_868F_9E81A1FED8AD

#include "compiler/ast/registry/registry_generic.h"
#include "compiler/ast/nodes/expression/i_expression_engine.h"
#include "compiler/lexer/token_type.h"

class RegistryExpression : public RegistryGeneric<TokenType, IExpressionEngine*>
{
public:
    RegistryExpression() = default;
    RegistryExpression(const RegistryExpression&) = delete;
    auto operator=(const RegistryExpression&) -> RegistryExpression& = delete;
    RegistryExpression(RegistryExpression&&) = delete;
    auto operator=(RegistryExpression&&) -> RegistryExpression& = delete;
    ~RegistryExpression() override = default;
};

#endif /* F5F52A3C_374A_47C1_868F_9E81A1FED8AD */
