#ifndef B9A25031_6CA1_409A_879D_68D43CAC9945
#define B9A25031_6CA1_409A_879D_68D43CAC9945

class NoeudScope;
class NoeudAffectationVariable;
class NoeudDeclarationVariable;
class NoeudVariable;
class NoeudAppelFonction;
class NoeudArgFonction;
class NoeudArgPassFonction;
class NoeudReturn;
class NoeudDeclarationFonction;
class Valeur;
class Instruction;
class Operation;

class IVisiteur
{
public:
    virtual void visiter(NoeudScope* NoeudScope) = 0;
    virtual void visiter(NoeudAffectationVariable* noeudAffectationVariable) = 0;
    virtual void visiter(NoeudDeclarationVariable* noeudDeclarationVariable) = 0; 
    virtual void visiter(NoeudVariable* noeudVariable) = 0; 
    virtual void visiter(NoeudAppelFonction* noeudAppelFonction) = 0;
    virtual void visiter(NoeudArgPassFonction* noeudArgPassFonction) = 0;
    virtual void visiter(NoeudReturn* noeudReturn) = 0; 
    virtual void visiter(NoeudArgFonction* noeudArgFonction) = 0 ;
    virtual void visiter(NoeudDeclarationFonction* noeudDeclarationFonction) = 0;
    virtual void visiter(Valeur* valeur) = 0; 
    virtual void visiter(Instruction* instruction) = 0; 
    virtual void visiter(Operation* operation) = 0; 
};

#endif /* B9A25031_6CA1_409A_879D_68D43CAC9945 */
