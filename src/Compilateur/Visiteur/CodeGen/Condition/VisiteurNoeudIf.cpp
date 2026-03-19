#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/Visiteur/CodeGen/Helper/ControlFlowHelper.h"
#include <llvm-18/llvm/IR/Value.h>

void VisiteurGeneralGenCode::visiter(NoeudIf* noeudIf) 
{
    // Récupérer la condition et les blocs à partir du noeud if
    INoeud* noeudCondition = noeudIf->getNoeudCondition();
    INoeud* noeudBlocIf = noeudIf->getNoeudBlocIf();
    INoeud* noeudBlocElse = noeudIf->getNoeudBlocElse();

    // Évaluer la condition
    noeudCondition->accept(this);
    llvm::Value* cmp = _contextGenCode->valeurTemporaire.adresse; 

    llvm::Function* fonctionEnCours = _contextGenCode->backend->getBuilder().GetInsertBlock()->getParent();

    auto [blocThen, blocElse, blocFin] = ControlFlowHelper::creerBlocsControle(fonctionEnCours, _contextGenCode->backend->getContext(), "if", "else", "endif");

    // Générer le code de branchement conditionnel
    _contextGenCode->backend->getBuilder().CreateCondBr(cmp, blocThen, blocElse);

    // Générer le code pour le bloc "if"
    _contextGenCode->backend->getBuilder().SetInsertPoint(blocThen);
    if (noeudBlocIf != nullptr) {
        noeudBlocIf->accept(this);
    }
    // Brancher vers endif à la fin du bloc if
    _contextGenCode->backend->getBuilder().CreateBr(blocFin);

    // Générer le code pour le bloc "else"
    _contextGenCode->backend->getBuilder().SetInsertPoint(blocElse);
    if (noeudBlocElse != nullptr) {
        noeudBlocElse->accept(this);
    }
    // Brancher vers endif à la fin du bloc else
    _contextGenCode->backend->getBuilder().CreateBr(blocFin);

    // Générer le code pour le bloc "endif"
    _contextGenCode->backend->getBuilder().SetInsertPoint(blocFin);
    
    // Car plus de noeud devrait traiter cette valeur temporaire après le if
    _contextGenCode->valeurTemporaire.adresse = nullptr;

}
