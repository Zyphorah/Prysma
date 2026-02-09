#ifndef FDDB5968_6DEF_49B0_AD11_8791D523278A
#define FDDB5968_6DEF_49B0_AD11_8791D523278A
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/Visiteur/Interfaces/IVisiteur.h"
#include "Compilateur/AST/Noeuds/NoeudInstruction.h"
#include <llvm-18/llvm/IR/Value.h>
#include "Compilateur/AST/Registre/ContextGenCode.h"
#include "Compilateur/AST/Noeuds/Equation/NoeudOperation.h"
#include <memory>

// Importation des classes 
class GenCodeAppelFonction;
class GenCodeArgFonction;
class GenCodeArgPassFonction;
class GenCodeDeclarationFonction;
class GenCodeReturn;
class GenCodeAffectationVariable;
class GenCodeDeclarationVariable;
class GenCodeValeur;
class GenCodeVariable;
class GenCodeOperation;
class GenCodeInstruction;
class GenCodeScope;
class NoeudOperation;

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
};

#endif /* FDDB5968_6DEF_49B0_AD11_8791D523278A */
