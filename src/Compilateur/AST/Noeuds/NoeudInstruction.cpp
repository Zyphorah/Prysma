#include "Compilateur/AST/Noeuds/NoeudInstruction.h"

NoeudInstruction::NoeudInstruction()
{
}

void NoeudInstruction::accept(IVisiteur* visiteur)
{
    visiteur->visiter(this);
}

void NoeudInstruction::ajouterInstruction(std::shared_ptr<INoeud> enfant)
{
    if (enfant) {
        enfants.push_back(enfant);
    }
}
