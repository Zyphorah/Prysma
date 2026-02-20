#include "Compilateur/AST/Noeuds/Condition/NoeudIf.h"
#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include <llvm-18/llvm/IR/Value.h>

void VisiteurGeneralGenCode::visiter(NoeudIf* noeudIf) 
{
    // Récupérer la condition et les blocs à partir du noeud if
    INoeud* noeudCondition = noeudIf->getNoeudCondition();
    INoeud* noeudBlocIf = noeudIf->getNoeudBlocIf();
    INoeud* noeudBlocElse = noeudIf->getNoeudBlocElse();

    // Évaluer la condition
    noeudCondition->accept(this);
    llvm::Value* cmp = _contextGenCode->valeurTemporaire; 

    llvm::Function* fonctionEnCours = _contextGenCode->backend->getBuilder().GetInsertBlock()->getParent();

    llvm::BasicBlock* blocThen = llvm::BasicBlock::Create(_contextGenCode->backend->getContext(), "if", fonctionEnCours);
    llvm::BasicBlock* blocElse = llvm::BasicBlock::Create(_contextGenCode->backend->getContext(), "else", fonctionEnCours);
    llvm::BasicBlock* blocFin = llvm::BasicBlock::Create(_contextGenCode->backend->getContext(), "endif", fonctionEnCours);

    // Générer le code de branchement conditionnel
    _contextGenCode->backend->getBuilder().CreateCondBr(cmp, blocThen, blocElse);

    // Générer le code pour le bloc "if"
    _contextGenCode->backend->getBuilder().SetInsertPoint(blocThen);
    if (noeudBlocIf) {
        noeudBlocIf->accept(this);
    }
    // Brancher vers endif à la fin du bloc if
    _contextGenCode->backend->getBuilder().CreateBr(blocFin);

    // Générer le code pour le bloc "else"
    _contextGenCode->backend->getBuilder().SetInsertPoint(blocElse);
    if (noeudBlocElse) {
        noeudBlocElse->accept(this);
    }
    // Brancher vers endif à la fin du bloc else
    _contextGenCode->backend->getBuilder().CreateBr(blocFin);

    // Générer le code pour le bloc "endif"
    _contextGenCode->backend->getBuilder().SetInsertPoint(blocFin);
    
    // Car plus de noeud devrait traiter cette valeur temporaire après le if
    _contextGenCode->valeurTemporaire = nullptr;

}
