#include "Compilateur/AST/Noeuds/Condition/NoeudBlocEndif.h"

NoeudBlocEndif::NoeudBlocEndif()
    = default;

NoeudBlocEndif::~NoeudBlocEndif()
{
}

void NoeudBlocEndif::accept(IVisiteur* visiteur) 
{
    visiteur->visiter(this);
}
