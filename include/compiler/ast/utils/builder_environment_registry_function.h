//===-- builder_environment_registry_function.h -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#ifndef BC96CA6B_A1FB_4C4F_AE37_A5D105CD4D4E
#define BC96CA6B_A1FB_4C4F_AE37_A5D105CD4D4E

#include "compiler/ast/registry/context_gen_code.h"
#include "compiler/ast/registry/registry_class.h"
#include "compiler/ast/registry/registry_function.h"
#include "compiler/ast/utils/i_builder_environment_registry.h"
#include <string>
#include <vector>

class BuilderEnvironmentRegistryFunction : public IBuilderEnvironmentRegistry
{
private:
    ContextGenCode* _contextGenCode; 
    void buildVTable(Class* classInfo, const std::string& className, const std::vector<NodeDeclarationFunction*>& parentMethodList);
    
public:
    explicit BuilderEnvironmentRegistryFunction(ContextGenCode* _contextGenCode);
    ~BuilderEnvironmentRegistryFunction() override;

    // Delete copy and move constructors and assignment operators
    BuilderEnvironmentRegistryFunction(const BuilderEnvironmentRegistryFunction&) = delete;
    auto operator=(const BuilderEnvironmentRegistryFunction&) -> BuilderEnvironmentRegistryFunction& = delete;
    BuilderEnvironmentRegistryFunction(BuilderEnvironmentRegistryFunction&&) = delete;
    auto operator=(BuilderEnvironmentRegistryFunction&&) -> BuilderEnvironmentRegistryFunction& = delete;

    void fill() override;
};

#endif /* BC96CA6B_A1FB_4C4F_AE37_A5D105CD4D4E */
