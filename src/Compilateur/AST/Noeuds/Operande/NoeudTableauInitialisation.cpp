#include "Compilateur/AST/Noeuds/Operande/NoeudTableauInitialisation.h"
#include "Compilateur/Visiteur/Interfaces/IVisiteur.h"
#include <utility>

NoeudTableauInitialisation::NoeudTableauInitialisation(std::vector<std::shared_ptr<INoeud>> elements)
    : _elements(std::move(elements))
{
}

void NoeudTableauInitialisation::accept(IVisiteur* visiteur)
{
    visiteur->visiter(this);
}
