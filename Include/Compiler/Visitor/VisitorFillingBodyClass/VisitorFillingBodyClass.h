#ifndef EC7E55EB_FEC0_4AFD_AEFD_1D5B3BCB89C0
#define EC7E55EB_FEC0_4AFD_AEFD_1D5B3BCB89C0

#include "Compiler/AST/Registry/ContextGenCode.h"
#include "Compiler/AST/Registry/RegistryClass.h"
#include "Compiler/Visitor/VisitorBaseGenerale.h"
#include <string>
#include <vector>

class FillingVisitorCoprsClass : public VisitorBaseGenerale
{
private:
    ContextGenCode* _contextGenCode; 
    using VisitorBaseGenerale::visiter;
    
    void construireVTable(Class* classInfo, const std::string& nomClass, 
                         const std::vector<NodeDeclarationFunction*>& listMethodeParent);
    
public:
    explicit FillingVisitorCoprsClass(ContextGenCode* contextGenCode);
    ~FillingVisitorCoprsClass() override;

    FillingVisitorCoprsClass(const FillingVisitorCoprsClass&) = delete;
    auto operator=(const FillingVisitorCoprsClass&) -> FillingVisitorCoprsClass& = delete;
    FillingVisitorCoprsClass(FillingVisitorCoprsClass&&) = delete;
    auto operator=(FillingVisitorCoprsClass&&) -> FillingVisitorCoprsClass& = delete;

    void visiter(NodeClass* nodeClass) override;

};

#endif /* EC7E55EB_FEC0_4AFD_AEFD_1D5B3BCB89C0 */
