#ifndef NOEUD_MAIN_H
#define NOEUD_MAIN_H

#include "Compilateur/AST/Noeuds/NoeudInstruction.h"
#include <llvm/IR/Value.h>
#include "Compilateur/Visiteur/AmisVisiteurs.h"

class NoeudScope : public NoeudInstruction
{
    LISTE_DES_AMIS_VISITEURS
public: 

    NoeudScope();
    ~NoeudScope();
    void accept(IVisiteur* visiteur) override;
};

#endif /* NOEUD_MAIN_H */
