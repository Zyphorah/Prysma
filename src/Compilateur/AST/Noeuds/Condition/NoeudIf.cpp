#include "Compilateur/AST/Noeuds/Condition/NoeudIf.h"
#include "Compilateur/Visiteur/Interfaces/IVisiteur.h"

NoeudIf::NoeudIf()
    = default;

NoeudIf::NoeudIf(INoeud* condition, INoeud* blocIf, INoeud* blocElse, INoeud* blocEndif)
    : noeudCondition(condition), noeudBlocIf(blocIf), noeudBlocElse(blocElse), noeudBlocEndif(blocEndif)
{
}

NoeudIf::~NoeudIf()
 {}
 
 void NoeudIf::accept(IVisiteur* visiteur) 
 {
    visiteur->visiter(this);
 }