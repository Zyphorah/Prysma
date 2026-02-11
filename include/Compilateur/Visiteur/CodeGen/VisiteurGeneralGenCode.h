#ifndef FDDB5968_6DEF_49B0_AD11_8791D523278A
#define FDDB5968_6DEF_49B0_AD11_8791D523278A


#include <llvm-18/llvm/IR/Value.h>
#include "Compilateur/AST/Registre/ContextGenCode.h"
#include "Compilateur/AST/Noeuds/Equation/NoeudOperation.h"
#include <memory>

//Outils 
class RegistreVariable;
class LlvmBackend;

class VisiteurGeneralGenCode : public IVisiteur
{
private:
    std::shared_ptr<ContextGenCode> _contextGenCode;
    
public:
    VisiteurGeneralGenCode(std::shared_ptr<ContextGenCode> contextGenCode);
    ~VisiteurGeneralGenCode();
    void parcourirEnfant(NoeudInstruction* noeud);
    
    llvm::Value* evaluerExpression(std::shared_ptr<INoeud>& expression);
    
    void visiter(NoeudScope* NoeudScope) override;
    void visiter(NoeudAffectationVariable* noeudAffectationVariable) override;
    void visiter(NoeudDeclarationVariable* noeudDeclarationVariable) override; 
    void visiter(NoeudAppelFonction* noeudAppelFonction) override;
    void visiter(NoeudRetour* noeudReturn) override;
    void visiter(NoeudArgFonction* noeudArgFonction) override;
    void visiter(NoeudDeclarationFonction* noeudDeclarationFonction) override;
    void visiter(NoeudOperation* noeudOperation) override;
    void visiter(NoeudLitteral* noeudLitteral) override;
    void visiter(NoeudInstruction* instruction) override;
    void visiter(NoeudUnRefVariable* noeudUnRefVariable) override;
    void visiter(NoeudRefVariable* noeudRefVariable) override;
    void visiter(NoeudIf* noeudIf) override;
    void visiter(NoeudElse* noeudIf) override;
};

#endif /* FDDB5968_6DEF_49B0_AD11_8791D523278A */
