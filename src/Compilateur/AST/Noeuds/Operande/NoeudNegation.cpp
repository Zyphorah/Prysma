#include "Compilateur/AST/Noeuds/Operande/NoeudNegation.h"
#include "Compilateur/Visiteur/Interfaces/IVisiteur.h"

void NoeudNegation::accept(IVisiteur* visiteur) {
    visiteur->visiter(this);
}
