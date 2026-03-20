#ifndef BC96CA6B_A1FB_4C4F_AE37_A5D105CD4D4E
#define BC96CA6B_A1FB_4C4F_AE37_A5D105CD4D4E

#include "Compilateur/AST/Registre/ContextGenCode.h"
#include "Compilateur/AST/Utils/IConstructeurEnvironnementRegistre.h"

class ConstructeurEnvironnementRegistreFonction : public IConstructeurEnvironnementRegistre
{
private:
    ContextGenCode* _contextGenCode; 

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
