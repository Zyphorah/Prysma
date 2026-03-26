#ifndef E5CE13D3_6823_4DDE_BB20_5311A0E477B6
#define E5CE13D3_6823_4DDE_BB20_5311A0E477B6
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Registry/ContextGenCode.h"
#include "Compiler/Visitor/VisitorBaseGenerale.h"

// This visitor is intended to fill the function and variable registries before code generation.
// It traverses the abstract syntax tree to fill the registries with nullptr values for functions and variables.
// This allows a first pass to populate the registries.

class OrchestratorInclude;

class FillingVisitorRegistry : public VisitorBaseGenerale
{
private:
    ContextGenCode* _contextGenCode;
    OrchestratorInclude* _orchestrator;
    
public:
    using VisitorBaseGenerale::visiter;
    FillingVisitorRegistry(ContextGenCode* contextGenCode, OrchestratorInclude* orchestrator);
    ~FillingVisitorRegistry() override;

    FillingVisitorRegistry(const FillingVisitorRegistry&) = delete;
    auto operator=(const FillingVisitorRegistry&) -> FillingVisitorRegistry& = delete;
    FillingVisitorRegistry(FillingVisitorRegistry&&) = delete;
    auto operator=(FillingVisitorRegistry&&) -> FillingVisitorRegistry& = delete;
    void visiter(NodeDeclarationVariable* nodeDeclarationVariable) override;
    void visiter(NodeDeclarationFunction* nodeDeclarationFunction) override;
    void visiter(NodeInclude* nodeInclude) override;
    void visiter(NodeClass* nodeClass) override;
};

#endif /* E5CE13D3_6823_4DDE_BB20_5311A0E477B6 */
