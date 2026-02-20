#include "Compilateur/AST/Noeuds/NoeudInstruction.h"
#include "Compilateur/Visiteur/Interfaces/IVisiteur.h"

NoeudInstruction::NoeudInstruction()
{
}

void NoeudInstruction::accept(IVisiteur* visiteur)
{
    visiteur->visiter(this);
}

void NoeudInstruction::ajouterInstruction(INoeud* enfant)
{
    if (enfant) {
        enfants.push_back(enfant);
    }
}
