#ifndef EF0EE0D7_D8CD_4A7D_BB3B_78249CA1F8AB
#define EF0EE0D7_D8CD_4A7D_BB3B_78249CA1F8AB

#include <llvm-18/llvm/IR/Value.h>
#include "Compilateur/GrapheVisuel/SortieGrapheVisuelTexte.h"
#include "Compilateur/Visiteur/VisiteurBaseGenerale.h"

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
class NoeudInclude;

class VisiteurGeneralGraphViz : public VisiteurBaseGenerale
{ 
private:
 SortieGrapheVisuelTexte _sortieGrapheVisuel;
 int _dernierId;
    
public:
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
    void visiter(NoeudInclude* noeudInclude) override;  
    VisiteurGeneralGraphViz(SortieGrapheVisuelTexte sortieGrapheVisuel);
    ~VisiteurGeneralGraphViz();
    void generer();
        
};

#endif /* FDDB5968_6DEF_49B0_AD11_8791D523278A */


