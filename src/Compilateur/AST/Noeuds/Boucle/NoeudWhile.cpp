#include "Compilateur/AST/Noeuds/Boucle/NoeudWhile.h"
#include "Compilateur/Visiteur/Interfaces/IVisiteur.h"

NoeudWhile::NoeudWhile()
    = default;

NoeudWhile::NoeudWhile(std::shared_ptr<INoeud>&& condition, std::shared_ptr<INoeud>&& blocWhile, std::shared_ptr<INoeud>&& blocEndWhile)
    : noeudCondition(std::move(condition)), noeudBlocWhile(std::move(blocWhile)), noeudBlocEndWhile(std::move(blocEndWhile))
{
}

NoeudWhile::~NoeudWhile()
{
}

void NoeudWhile::accept(IVisiteur* visiteur) 
{
    visiteur->visiter(this);
}

void NoeudWhile::ajouterInstruction(std::shared_ptr<INoeud> enfant)
{
    enfants.push_back(std::move(enfant));
}