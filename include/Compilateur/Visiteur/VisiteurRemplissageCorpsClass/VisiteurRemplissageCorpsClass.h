#ifndef EC7E55EB_FEC0_4AFD_AEFD_1D5B3BCB89C0
#define EC7E55EB_FEC0_4AFD_AEFD_1D5B3BCB89C0

#include "Compilateur/AST/Registre/ContextGenCode.h"
#include "Compilateur/AST/Registre/RegistreClass.h"
#include "Compilateur/Visiteur/VisiteurBaseGenerale.h"
#include <string>
#include <vector>

class VisiteurRemplissageCoprsClass : public VisiteurBaseGenerale
{
private:
    ContextGenCode* _contextGenCode; 
    using VisiteurBaseGenerale::visiter;
    
    void construireVTable(Class* classInfo, const std::string& nomClass, 
                         const std::vector<NoeudDeclarationFonction*>& listMethodeParent);
    
public:
    explicit VisiteurRemplissageCoprsClass(ContextGenCode* contextGenCode);
    ~VisiteurRemplissageCoprsClass() override;

    VisiteurRemplissageCoprsClass(const VisiteurRemplissageCoprsClass&) = delete;
    auto operator=(const VisiteurRemplissageCoprsClass&) -> VisiteurRemplissageCoprsClass& = delete;
    VisiteurRemplissageCoprsClass(VisiteurRemplissageCoprsClass&&) = delete;
    auto operator=(VisiteurRemplissageCoprsClass&&) -> VisiteurRemplissageCoprsClass& = delete;

    void visiter(NoeudClass* noeudClass) override;

};

#endif /* EC7E55EB_FEC0_4AFD_AEFD_1D5B3BCB89C0 */
