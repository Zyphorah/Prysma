#ifndef A1014A88_A2B2_4637_B4CD_3709FCBE042F
#define A1014A88_A2B2_4637_B4CD_3709FCBE042F

#include "Compiler/AST/Registry/ContextGenCode.h"
#include "Compiler/AST/Utils/IBuilderEnvironmentRegistry.h"


class BuilderEnvironmentRegistryVariable : public IBuilderEnvironmentRegistry
{
private:
    ContextGenCode* _contextGenCode;


public:
    explicit BuilderEnvironmentRegistryVariable(ContextGenCode* _contextGenCode);
    ~BuilderEnvironmentRegistryVariable() override;

    BuilderEnvironmentRegistryVariable(const BuilderEnvironmentRegistryVariable&) = delete;
    auto operator=(const BuilderEnvironmentRegistryVariable&) -> BuilderEnvironmentRegistryVariable& = delete;

    BuilderEnvironmentRegistryVariable(BuilderEnvironmentRegistryVariable&&) = delete;
    auto operator=(BuilderEnvironmentRegistryVariable&&) -> BuilderEnvironmentRegistryVariable& = delete;

    void remplir() override;

};

#endif /* A1014A88_A2B2_4637_B4CD_3709FCBE042F */
