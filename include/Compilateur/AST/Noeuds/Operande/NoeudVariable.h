#ifndef C9126446_60DE_4831_AEE0_655AF47AE40D
#define C9126446_60DE_4831_AEE0_655AF47AE40D

#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/GestionnaireChargementVariable.h"
#include <memory>
#include <string>

class NoeudVariable : public INoeud
{
private:
    llvm::Value* _adresseMemoire;
    std::string _nomVariable;
    std::shared_ptr<GestionnaireChargementVariable> _gestionnaire;

public:
    NoeudVariable(llvm::Value* adresseMemoire, const std::string& nomVariable, std::shared_ptr<GestionnaireChargementVariable> gestionnaire);
    ~NoeudVariable();

    llvm::Value* genCode() override;
};


#endif /* C9126446_60DE_4831_AEE0_655AF47AE40D */
