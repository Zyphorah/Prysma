#ifndef B9A25031_6CA1_409A_879D_68D43CAC9945
#define B9A25031_6CA1_409A_879D_68D43CAC9945

class NoeudScope;
class NoeudAffectationVariable;
class NoeudDeclarationVariable;
class NoeudVariable;
class NoeudAppelFonction;
class NoeudArgFonction;
class NoeudArgPassFonction;
class NoeudRetour;
class NoeudDeclarationFonction;
class NoeudOperation;
class Valeur;
class NoeudLitteral;
class NoeudInstruction;
class NoeudUnRefVariable;
class NoeudRefVariable;
class NoeudIf;
class NoeudWhile;
class NoeudNegation;

class IVisiteur
{
public:
    virtual void visiter(NoeudScope* NoeudScope) = 0;
    virtual void visiter(NoeudAffectationVariable* noeudAffectationVariable) = 0;
    virtual void visiter(NoeudDeclarationVariable* noeudDeclarationVariable) = 0; 
    virtual void visiter(NoeudAppelFonction* noeudAppelFonction) = 0;
    virtual void visiter(NoeudRetour* noeudReturn) = 0; 
    virtual void visiter(NoeudArgFonction* noeudArgFonction) = 0 ;
    virtual void visiter(NoeudDeclarationFonction* noeudDeclarationFonction) = 0;
    virtual void visiter(NoeudOperation* noeudOperation) = 0;
    virtual void visiter(NoeudLitteral* noeudLitteral) = 0;
    virtual void visiter(NoeudInstruction* instruction) = 0; 
    virtual void visiter(NoeudUnRefVariable* noeudUnRefVariable) = 0;
    virtual void visiter(NoeudRefVariable* noeudRefVariable) = 0;
    virtual void visiter(NoeudIf* noeudIf) = 0;
    virtual void visiter(NoeudWhile* noeudWhile) = 0;
    virtual void visiter(NoeudNegation* noeudNegation) = 0;
};

#endif /* B9A25031_6CA1_409A_879D_68D43CAC9945 */
