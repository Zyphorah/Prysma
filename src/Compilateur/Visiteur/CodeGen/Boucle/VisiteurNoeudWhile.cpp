#include "Compilateur/AST/Noeuds/Boucle/NoeudWhile.h"
#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"

void VisiteurGeneralGenCode::visiter(NoeudWhile* noeudWhile) 
{
    // Récupérer la condition et les blocs à partir du noeud while
    INoeud* noeudCondition = noeudWhile->getNoeudCondition();
    INoeud* noeudBlocWhile = noeudWhile->getNoeudBlocWhile();
    INoeud* noeudBlocEndWhile = noeudWhile->getNoeudBlocFinWhile();
    
    // construire les blocs de base pour le while 
    llvm::Function* fonctionEnCours = _contextGenCode->backend->getBuilder().GetInsertBlock()->getParent();
    llvm::BasicBlock* blocCondition = llvm::BasicBlock::Create(_contextGenCode->backend->getContext(), "while.cond", fonctionEnCours);
    llvm::BasicBlock* blocWhile = llvm::BasicBlock::Create(_contextGenCode->backend->getContext(), "while.body", fonctionEnCours);
    llvm::BasicBlock* blocEndWhile = llvm::BasicBlock::Create(_contextGenCode->backend->getContext(), "while.end", fonctionEnCours);

    // Générer le code pour la condition du while while.cond
    _contextGenCode->backend->getBuilder().CreateBr(blocCondition);
    _contextGenCode->backend->getBuilder().SetInsertPoint(blocCondition); 
    noeudCondition->accept(this);
    llvm::Value* cmp = _contextGenCode->valeurTemporaire;
    _contextGenCode->backend->getBuilder().CreateCondBr(cmp, blocWhile, blocEndWhile);

    // Générer le code pour le bloc du while while.body

    _contextGenCode->backend->getBuilder().SetInsertPoint(blocWhile);
    if (noeudBlocWhile) {
        noeudBlocWhile->accept(this);
        _contextGenCode->backend->getBuilder().CreateBr(blocCondition);

    }
    _contextGenCode->backend->getBuilder().SetInsertPoint(blocEndWhile);
}