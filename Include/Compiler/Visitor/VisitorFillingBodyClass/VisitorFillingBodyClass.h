#ifndef EC7E55EB_FEC0_4AFD_AEFD_1D5B3BCB89C0
#define EC7E55EB_FEC0_4AFD_AEFD_1D5B3BCB89C0

#include "Compiler/AST/Registry/ContextGenCode.h"
#include "Compiler/AST/Registry/RegistryClass.h"
#include "Compiler/Visitor/VisitorBaseGenerale.h"
#include <string>
#include <vector>

// Visitor that fills the body of a class (methods, vtable, etc.)
class FillingVisitorBodyClass : public VisitorBaseGenerale
{
private:
    ContextGenCode* _contextGenCode; 
    using VisitorBaseGenerale::visiter;
    
    void buildVTable(Class* classInfo, const std::string& className, 
                     const std::vector<NodeDeclarationFunction*>& parentMethodList);
    
public:
    explicit FillingVisitorBodyClass(ContextGenCode* contextGenCode);
    ~FillingVisitorBodyClass() override;

    FillingVisitorBodyClass(const FillingVisitorBodyClass&) = delete;
    auto operator=(const FillingVisitorBodyClass&) -> FillingVisitorBodyClass& = delete;
    FillingVisitorBodyClass(FillingVisitorBodyClass&&) = delete;
    auto operator=(FillingVisitorBodyClass&&) -> FillingVisitorBodyClass& = delete;

    void visiter(NodeClass* nodeClass) override;
};

#endif /* EC7E55EB_FEC0_4AFD_AEFD_1D5B3BCB89C0 */
