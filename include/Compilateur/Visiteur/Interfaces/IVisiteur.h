#ifndef IVISITEUR_H
#define IVISITEUR_H

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

class IVisiteur
{
public:
    virtual ~IVisiteur() = default;
    virtual void visiter(NoeudInclude* noeudInclude) = 0;
    virtual void visiter(NoeudDeclarationVariable* noeudDeclarationVariable) = 0;
    virtual void visiter(NoeudDeclarationFonction* noeudDeclarationFonction) = 0;
    virtual void visiter(NoeudArgFonction* noeudArgFonction) = 0;
    virtual void visiter(NoeudAffectationVariable* noeudAffectationVariable) = 0;
    virtual void visiter(NoeudAffectationTableau* noeudAffectationTableau) = 0;
    virtual void visiter(NoeudRetour* noeudRetour) = 0;
    virtual void visiter(NoeudIf* noeudIf) = 0;
    virtual void visiter(NoeudWhile* noeudWhile) = 0;
    virtual void visiter(NoeudOperation* noeudOperation) = 0;
    virtual void visiter(NoeudLitteral* noeudLitteral) = 0;
    virtual void visiter(NoeudAppelFonction* noeudAppelFonction) = 0;
    virtual void visiter(NoeudLectureTableau* noeudLectureTableau) = 0;
    virtual void visiter(NoeudTableauInitialisation* noeudTableauInitialisation) = 0;
    virtual void visiter(NoeudNegation* noeudNegation) = 0;
    virtual void visiter(NoeudRefVariable* noeudRefVariable) = 0;
    virtual void visiter(NoeudUnRefVariable* noeudUnRefVariable) = 0;
    virtual void visiter(NoeudNew* noeudNew) = 0;
    virtual void visiter(NoeudInstruction* noeudInstruction) = 0;
};

#endif
