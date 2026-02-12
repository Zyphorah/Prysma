#ifndef NOEUD_BLOC_IF_H
#define NOEUD_BLOC_IF_H

#include "Compilateur/AST/Noeuds/NoeudInstruction.h"

class NoeudBlocIf : public NoeudInstruction
{
private: 

public: 
    NoeudBlocIf();
    ~NoeudBlocIf();
    
    void accept(IVisiteur* visiteur) override;
};

#endif /* NOEUD_BLOC_IF_H */
