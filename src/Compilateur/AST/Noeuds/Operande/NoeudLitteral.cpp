#include "Compilateur/AST/Noeuds/Operande/NoeudLitteral.h"

NoeudLitteral::NoeudLitteral(float valeur)
    : _valeur(valeur)
{
}

void NoeudLitteral::accept(IVisiteur* visiteur)
{
    visiteur->visiter(this);
}
