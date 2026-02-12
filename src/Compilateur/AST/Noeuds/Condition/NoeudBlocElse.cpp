#include "Compilateur/AST/Noeuds/Condition/NoeudBlocElse.h"

NoeudBlocElse::NoeudBlocElse()
    = default;

NoeudBlocElse::~NoeudBlocElse()
{
}

void NoeudBlocElse::accept(IVisiteur* visiteur) 
{
    visiteur->visiter(this);
}
