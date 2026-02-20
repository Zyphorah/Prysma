#ifndef FDDB5968_6DEF_49B0_AD11_8791D523278A
#define FDDB5968_6DEF_49B0_AD11_8791D523278A

#include <llvm-18/llvm/IR/Value.h>
#include "Compilateur/AST/Registre/ContextGenCode.h"
#include "Compilateur/Visiteur/Interfaces/IVisiteur.h"
#include "Compilateur/Visiteur/MacroGenerationVisiteur.h"
#include <memory>


CLASS_NOEUD

class RegistreVariable; 
class LlvmBackend; 

class VisiteurGeneralGenCode : public IVisiteur
{
private:
    ContextGenCode* _contextGenCode;
    
public:
    VisiteurGeneralGenCode(ContextGenCode* contextGenCode);
    ~VisiteurGeneralGenCode();
    void parcourirEnfant(NoeudInstruction* noeud);
    
    llvm::Value* evaluerExpression(INoeud* expression);
    
    DECLARER_METHODES_VISITEUR
};

#endif /* FDDB5968_6DEF_49B0_AD11_8791D523278A */
