#ifndef B7984297_5521_450F_A453_2594C7FDFC7A
#define B7984297_5521_450F_A453_2594C7FDFC7A

#include "Compilateur/AST/Noeuds/NoeudInstruction.h"
class NoeudIf : public NoeudInstruction
{
private: 
    INoeud* noeudCondition;
    INoeud* noeudBlocIf;
    INoeud* noeudBlocElse;
    INoeud* noeudBlocEndif;

public: 
    NoeudIf();
    NoeudIf(INoeud* condition, INoeud* blocIf, INoeud* blocElse, INoeud* blocEndif);
    ~NoeudIf();
    
    void accept(IVisiteur* visiteur) override;

    INoeud* getNoeudCondition() const { return noeudCondition; }
    INoeud* getNoeudBlocIf() const { return noeudBlocIf; }
    INoeud* getNoeudBlocElse() const { return noeudBlocElse; }
    INoeud* getNoeudBlocEndif() const { return noeudBlocEndif; }
};

#endif /* B7984297_5521_450F_A453_2594C7FDFC7A */
