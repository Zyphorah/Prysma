#include "Compilateur/AST/Noeuds/Condition/NoeudElse.h"

NoeudElse::NoeudElse()
{}

NoeudElse::~NoeudElse()
{}


void NoeudElse::accept(IVisiteur* visiteur)
{
    visiteur->visiter(this);
}