#ifndef B7984297_5521_450F_A453_2594C7FDFC7A
#define B7984297_5521_450F_A453_2594C7FDFC7A

#include "Compilateur/AST/Noeuds/NoeudInstruction.h"
class NoeudIf : public NoeudInstruction
{
private: 

public: 
    NoeudIf();
    ~NoeudIf();
    
    void accept(IVisiteur* visiteur) override;
};

#endif /* B7984297_5521_450F_A453_2594C7FDFC7A */
