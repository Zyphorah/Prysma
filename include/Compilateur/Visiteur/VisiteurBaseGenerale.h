#ifndef VISITEUR_BASE_GENERALE_H
#define VISITEUR_BASE_GENERALE_H

#include "Compilateur/Visiteur/Interfaces/IVisiteur.h"
#include "Compilateur/AST/AST_Genere.h"

class NoeudInclude;
class NoeudDeclarationVariable;
class NoeudDeclarationFonction;
class NoeudArgFonction;
class NoeudAffectationVariable;
class NoeudAffectationTableau;
class NoeudRetour;
class NoeudIf;
class NoeudWhile;
class NoeudOperation;
class NoeudLitteral;
class NoeudAppelFonction;
class NoeudLectureTableau;
class NoeudTableauInitialisation;
class NoeudNegation;
class NoeudRefVariable;
class NoeudUnRefVariable;
class NoeudNew;
class NoeudInstruction;

class VisiteurBaseGenerale : public IVisiteur
{
public:
    void visiter(NoeudInclude* noeudInclude) override;
    void visiter(NoeudDeclarationVariable* noeudDeclarationVariable) override;
    void visiter(NoeudDeclarationFonction* noeudDeclarationFonction) override;
    void visiter(NoeudArgFonction* noeudArgFonction) override;
    void visiter(NoeudAffectationVariable* noeudAffectationVariable) override;
    void visiter(NoeudAffectationTableau* noeudAffectationTableau) override;
    void visiter(NoeudRetour* noeudRetour) override;
    void visiter(NoeudIf* noeudIf) override;
    void visiter(NoeudWhile* noeudWhile) override;
    void visiter(NoeudOperation* noeudOperation) override;
    void visiter(NoeudLitteral* noeudLitteral) override;
    void visiter(NoeudAppelFonction* noeudAppelFonction) override;
    void visiter(NoeudLectureTableau* noeudLectureTableau) override;
    void visiter(NoeudTableauInitialisation* noeudTableauInitialisation) override;
    void visiter(NoeudNegation* noeudNegation) override;
    void visiter(NoeudRefVariable* noeudRefVariable) override;
    void visiter(NoeudUnRefVariable* noeudUnRefVariable) override;
    void visiter(NoeudNew* noeudNew) override;
    void visiter(NoeudInstruction* noeudInstruction) override;
};

#endif
