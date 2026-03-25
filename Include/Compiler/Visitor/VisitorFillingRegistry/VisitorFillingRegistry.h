#ifndef E5CE13D3_6823_4DDE_BB20_5311A0E477B6
#define E5CE13D3_6823_4DDE_BB20_5311A0E477B6
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Registry/ContextGenCode.h"
#include "Compiler/Visitor/VisitorBaseGenerale.h"

// Ce visitor a pour but de remplir les registrys de function et de variable avant la génération du code, 
// Il va parcourir l'tree syntaxique abstrait pour remplir les registrys avec des valeur nullptr pour les function et les variables
// Cela permet d'avoir une première passe pour remplir les registrys. 

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
