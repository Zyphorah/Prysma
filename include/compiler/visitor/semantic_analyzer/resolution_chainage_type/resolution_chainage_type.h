#include "compiler/ast/ast_genere.h"
#include "compiler/ast/registry/registry_class.h"
#include "compiler/visitor/visitor_base_generale.h"

class ResolutionChainageType : public VisitorBaseGenerale
{

private:
    RegistryClass* _registryClass;

public: 
    explicit ResolutionChainageType(RegistryClass* registryClass) : _registryClass(registryClass) {}
    ~ResolutionChainageType() override = default;

    // Delete copy and move constructors and assignment operators
    ResolutionChainageType(const ResolutionChainageType&) = delete;
    auto operator=(const ResolutionChainageType&) -> ResolutionChainageType& = delete;
    ResolutionChainageType(ResolutionChainageType&&) = delete;
    auto operator=(ResolutionChainageType&&) -> ResolutionChainageType& = delete;
    
    void visiter(NodeCallObject *nodeCallObject) override;
};