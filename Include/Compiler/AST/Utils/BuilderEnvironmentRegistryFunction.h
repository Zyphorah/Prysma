#ifndef BC96CA6B_A1FB_4C4F_AE37_A5D105CD4D4E
#define BC96CA6B_A1FB_4C4F_AE37_A5D105CD4D4E

#include "Compiler/AST/Registry/ContextGenCode.h"
#include "Compiler/AST/Registry/RegistryClass.h"
#include "Compiler/AST/Registry/RegistryFunction.h"
#include "Compiler/AST/Utils/IBuilderEnvironmentRegistry.h"
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
