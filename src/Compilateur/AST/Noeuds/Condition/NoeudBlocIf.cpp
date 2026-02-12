#include "Compilateur/AST/Noeuds/Condition/NoeudBlocIf.h"

NoeudBlocIf::NoeudBlocIf()
    = default;

NoeudBlocIf::~NoeudBlocIf()
{
}

void NoeudBlocIf::accept(IVisiteur* visiteur) 
{
    visiteur->visiter(this);
}
