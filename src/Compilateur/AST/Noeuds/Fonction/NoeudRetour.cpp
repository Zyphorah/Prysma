#include "Compilateur/AST/Noeuds/Fonction/NoeudRetour.h"
#include "Compilateur/Visiteur/Interfaces/IVisiteur.h"
#include <llvm-18/llvm/IR/Instructions.h>
#include <utility>

NoeudRetour::NoeudRetour(INoeud* valeurRetour)
    : _valeurRetour(valeurRetour)
{
}

 // Implémentation sera géré par le parent 
void NoeudRetour::accept(IVisiteur* visiteur)
{
    visiteur->visiter(this);
}

