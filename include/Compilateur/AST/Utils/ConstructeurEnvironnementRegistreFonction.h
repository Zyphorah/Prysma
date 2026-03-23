#ifndef BC96CA6B_A1FB_4C4F_AE37_A5D105CD4D4E
#define BC96CA6B_A1FB_4C4F_AE37_A5D105CD4D4E

#include "Compilateur/AST/Registre/ContextGenCode.h"
#include "Compilateur/AST/Registre/RegistreClass.h"
#include "Compilateur/AST/Registre/RegistreFonction.h"
#include "Compilateur/AST/Utils/IConstructeurEnvironnementRegistre.h"
#include <string>
#include <vector>

class ConstructeurEnvironnementRegistreFonction : public IConstructeurEnvironnementRegistre
{
private:
    ContextGenCode* _contextGenCode; 
    void construireVTable(Class* classInfo, const std::string& nomClass, const std::vector<NoeudDeclarationFonction*>& listMethodeParent);
    
public:
    explicit ConstructeurEnvironnementRegistreFonction(ContextGenCode* _contextGenCode);
    ~ConstructeurEnvironnementRegistreFonction() override;

    // Delete copy and move constructors and assignment operators
    ConstructeurEnvironnementRegistreFonction(const ConstructeurEnvironnementRegistreFonction&) = delete;
    auto operator=(const ConstructeurEnvironnementRegistreFonction&) -> ConstructeurEnvironnementRegistreFonction& = delete;
    ConstructeurEnvironnementRegistreFonction(ConstructeurEnvironnementRegistreFonction&&) = delete;
    auto operator=(ConstructeurEnvironnementRegistreFonction&&) -> ConstructeurEnvironnementRegistreFonction& = delete;

    void remplir() override;
};

#endif /* BC96CA6B_A1FB_4C4F_AE37_A5D105CD4D4E */
