#ifndef E076A996_22DF_4047_84EE_673A3325558D
#define E076A996_22DF_4047_84EE_673A3325558D

#include "Compilateur/AST/Noeuds/NoeudInstruction.h"

class NoeudBlocElse : public NoeudInstruction
{
private: 

public: 
    NoeudBlocElse();
    ~NoeudBlocElse();
    
    void accept(IVisiteur* visiteur) override;
};

#endif /* E076A996_22DF_4047_84EE_673A3325558D */
