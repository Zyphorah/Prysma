#ifndef EC7E55EB_FEC0_4AFD_AEFD_1D5B3BCB89C0
#define EC7E55EB_FEC0_4AFD_AEFD_1D5B3BCB89C0

#include "Compilateur/AST/Registre/ContextGenCode.h"
#include "Compilateur/Visiteur/VisiteurBaseGenerale.h"

class VisiteurRemplissageCoprsClass : public VisiteurBaseGenerale
{
private:
    ContextGenCode* _contextGenCode; 
    using VisiteurBaseGenerale::visiter;
    
    void construireVTable(Class* classInfo, const std::string& nomClass, 
                         const std::vector<INoeud*>& listMethodeParent);
    
public:
    VisiteurRemplissageCoprsClass(ContextGenCode* contextGenCode);
    ~VisiteurRemplissageCoprsClass();
    void visiter(NoeudClass* noeudClass) override;

};

#endif /* EC7E55EB_FEC0_4AFD_AEFD_1D5B3BCB89C0 */
