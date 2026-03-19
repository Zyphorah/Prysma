#ifndef FDDB5968_6DEF_49B0_AD11_8791D523278A
#define FDDB5968_6DEF_49B0_AD11_8791D523278A

#include <llvm-18/llvm/IR/Value.h>
#include "Compilateur/AST/Registre/ContextGenCode.h"
#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include "Compilateur/Visiteur/Interfaces/IVisiteur.h"
#include "Compilateur/Visiteur/VisiteurBaseGenerale.h"


class RegistreVariable; 
class LlvmBackend; 
class OrchestrateurInclude;
class NoeudInstruction;
class NoeudAffectationVariable;
class NoeudDeclarationVariable;
class NoeudAppelFonction;
class NoeudRetour;
class NoeudArgFonction;
class NoeudDeclarationFonction;
class NoeudLitteral;
class NoeudUnRefVariable;
class NoeudRefVariable;
class NoeudIf;
class NoeudElse;
class NoeudWhile;
class NoeudOperation;
class NoeudNegation;
class NoeudTableauInitialisation;
class NoeudAffectationTableau;
class NoeudLectureTableau;
class NoeudNew;
class NoeudDelete;
class NoeudAppelObjet;

class VisiteurGeneralGenCode : public VisiteurBaseGenerale
{
private:
    ContextGenCode* _contextGenCode;
    OrchestrateurInclude* _orchestrateurInclude;
    
public:
    using VisiteurBaseGenerale::visiter;
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
    void visiter(NoeudWhile* noeudWhile) override;
    void visiter(NoeudNegation* noeudNegation) override;
    void visiter(NoeudTableauInitialisation* noeudTableauInit) override;
    void visiter(NoeudAffectationTableau* noeudAffectationTableau) override;
    void visiter(NoeudLectureTableau* noeudLectureTableau) override;
    void visiter(NoeudNew* noeudNew) override;
    void visiter(NoeudDelete* noeudDelete) override;
    void visiter(NoeudClass* noeudClass) override;
    void visiter(NoeudAppelObjet* noeudAppelObjet) override;
    void visiter(NoeudAccesAttribut* noeudAccesAttribut) override;
    VisiteurGeneralGenCode(ContextGenCode* contextGenCode, OrchestrateurInclude* orchestrateurInclude);
    virtual ~VisiteurGeneralGenCode();
    void parcourirEnfant(NoeudInstruction* noeud);
    
    Symbole evaluerExpression(INoeud* expression);
    
    static std::string obtenirNomClasseDepuisSymbole(const Symbole& objetSymbole);
};

#endif /* FDDB5968_6DEF_49B0_AD11_8791D523278A */
