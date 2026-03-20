#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/Visiteur/CodeGen/Helper/ControlFlowHelper.h"
#include <llvm-18/llvm/IR/Function.h>
#include <llvm/IR/Value.h>

void VisiteurGeneralGenCode::visiter(NoeudWhile* noeudWhile) 
{
    // Récupérer la condition et les blocs à partir du noeud while
    INoeud* noeudCondition = noeudWhile->getNoeudCondition();
    INoeud* noeudBlocWhile = noeudWhile->getNoeudBlocWhile();
    INoeud* noeudBlocEndWhile = noeudWhile->getNoeudBlocFinWhile();
    
    // construire les blocs de base pour le while 
    llvm::Function* fonctionEnCours = _contextGenCode->getBackend()->getBuilder().GetInsertBlock()->getParent();
    auto [blocCondition, blocWhile, blocEndWhile] = ControlFlowHelper::creerBlocsControle(fonctionEnCours, _contextGenCode->getBackend()->getContext(), "while.cond", "while.body", "while.end");

    // Générer le code pour la condition du while while.cond
    _contextGenCode->getBackend()->getBuilder().CreateBr(blocCondition);
    _contextGenCode->getBackend()->getBuilder().SetInsertPoint(blocCondition); 
    noeudCondition->accept(this);
    llvm::Value* cmp = _contextGenCode->getValeurTemporaire().getAdresse();
    _contextGenCode->getBackend()->getBuilder().CreateCondBr(cmp, blocWhile, blocEndWhile);

    // Générer le code pour le bloc du while while.body

    _contextGenCode->getBackend()->getBuilder().SetInsertPoint(blocWhile);
    if (noeudBlocWhile != nullptr) {
        noeudBlocWhile->accept(this);
        _contextGenCode->getBackend()->getBuilder().CreateBr(blocCondition);

    }
    _contextGenCode->getBackend()->getBuilder().SetInsertPoint(blocEndWhile);
    if (noeudBlocEndWhile != nullptr) {
        noeudBlocEndWhile->accept(this);
    }
}