#include "Compilateur/AST/Noeuds/Boucle/NoeudWhile.h"
#include "Compilateur/Visiteur/Interfaces/IVisiteur.h"

NoeudWhile::NoeudWhile()
    = default;

NoeudWhile::NoeudWhile(INoeud* condition, INoeud* blocWhile, INoeud* blocEndWhile)
    : noeudCondition(condition), noeudBlocWhile(blocWhile), noeudBlocFinWhile(blocEndWhile)
{
}

NoeudWhile::~NoeudWhile()
{
}

void NoeudWhile::accept(IVisiteur* visiteur) 
{
    visiteur->visiter(this);
}
