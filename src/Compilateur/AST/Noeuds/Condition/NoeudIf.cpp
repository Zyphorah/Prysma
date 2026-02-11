#include "Compilateur/AST/Noeuds/Condition/NoeudIf.h"

 NoeudIf::NoeudIf()
 = default;

NoeudIf::~NoeudIf()
 {}
 
 void NoeudIf::accept(IVisiteur* visiteur) 
 {
    visiteur->visiter(this);
 }