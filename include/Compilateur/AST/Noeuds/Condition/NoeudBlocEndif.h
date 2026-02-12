#ifndef B9BA30C5_BA04_46DF_9020_6833386DB66F
#define B9BA30C5_BA04_46DF_9020_6833386DB66F

#include "Compilateur/AST/Noeuds/NoeudInstruction.h"

class NoeudBlocEndif : public NoeudInstruction
{
private: 

public: 
    NoeudBlocEndif();
    ~NoeudBlocEndif();
    
    void accept(IVisiteur* visiteur) override;
};

#endif /* B9BA30C5_BA04_46DF_9020_6833386DB66F */
