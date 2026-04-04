//===-- context_parser.h -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#ifndef F4099BCE_4896_40B4_B34C_93B4827706C3
#define F4099BCE_4896_40B4_B34C_93B4827706C3

#include "compiler/ast/interfaces/i_builder_tree.h"
#include "compiler/ast/registry/stack/registry_variable.h"
#include "compiler/ast/registry/registry_type.h"
#include "compiler/parser/parser_type.h"
#include <stdexcept>

struct ContextParser
{
private:
    IBuilderTree* builderTreeEquation;
    IBuilderTree* builderTreeInstruction;
    TypeParser* parserType;
    RegistryVariable* registryVariable;
    RegistryType* registryType;

public:
    struct Dependencies {
        IBuilderTree* builderTreeEquation;
        IBuilderTree* builderTreeInstruction;
        TypeParser* parserType;
        RegistryVariable* registryVariable;
        RegistryType* registryType;
    };

    explicit ContextParser(const Dependencies& deps)
        : builderTreeEquation(deps.builderTreeEquation),
          builderTreeInstruction(deps.builderTreeInstruction),
          parserType(deps.parserType),
          registryVariable(deps.registryVariable),
          registryType(deps.registryType)
    {
        if(builderTreeEquation == nullptr)
        {
            throw std::invalid_argument("builderTreeEquation cannot be null");
        }

        if(builderTreeInstruction == nullptr)
        {
            throw std::invalid_argument("builderTreeInstruction cannot be null");
        }

        if(parserType == nullptr)
        {
            throw std::invalid_argument("parserType cannot be null");
        }

        if(registryVariable == nullptr)
        {
            throw std::invalid_argument("registryVariable cannot be null");
        }

        if(registryType == nullptr)
        {
            throw std::invalid_argument("registryType cannot be null");
        }
    }    

    [[nodiscard]] auto getBuilderTreeEquation() const -> IBuilderTree* { return builderTreeEquation; }
    [[nodiscard]] auto getBuilderTreeInstruction() const -> IBuilderTree* { return builderTreeInstruction; }
    [[nodiscard]] auto getTypeParser() const -> TypeParser* { return parserType; }
    [[nodiscard]] auto getRegistryVariable() const -> RegistryVariable* { return registryVariable; }
    [[nodiscard]] auto getRegistryType() const -> RegistryType* { return registryType; }
};

#endif /* F4099BCE_4896_40B4_B34C_93B4827706C3 */
