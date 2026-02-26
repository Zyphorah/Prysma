#ifndef FDDB5968_6DEF_49B0_AD11_8791D523278A
#define FDDB5968_6DEF_49B0_AD11_8791D523278A

#include <llvm-18/llvm/IR/Value.h>
#include "Compilateur/AST/Registre/ContextGenCode.h"
#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include "Compilateur/Visiteur/Interfaces/IVisiteur.h"
#include "Compilateur/Visiteur/MacroGenerationVisiteur.h"
#include "Compilateur/Visiteur/VisiteurBaseGenerale.h"


class RegistreVariable; 
class LlvmBackend; 
class OrchestrateurInclude;
CLASS_NOEUD

class VisiteurGeneralGenCode : public VisiteurBaseGenerale
{
private:
    ContextGenCode* _contextGenCode;
    OrchestrateurInclude* _orchestrateurInclude;
    
public:
    using VisiteurBaseGenerale::visiter;
    DECLARER_METHODES_VISITEUR
    VisiteurGeneralGenCode(ContextGenCode* contextGenCode, OrchestrateurInclude* orchestrateurInclude);
    virtual ~VisiteurGeneralGenCode();
    void parcourirEnfant(NoeudInstruction* noeud);
    
    Symbole evaluerExpression(INoeud* expression);
};

#endif /* FDDB5968_6DEF_49B0_AD11_8791D523278A */
