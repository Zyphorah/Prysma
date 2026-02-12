#include "Compilateur/AST/Noeuds/NoeudInstruction.h"
#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include <llvm-18/llvm/IR/Value.h>

void VisiteurGeneralGenCode::visiter(NoeudIf* noeudIf) 
{
    // Récupérer le registre cmp pour la condition 

   llvm::Value* cmp = _contextGenCode->valeurTemporaire; 

    llvm::Function* fonctionEnCours = _contextGenCode->backend->getBuilder().GetInsertBlock()->getParent();

    llvm::BasicBlock* blocThen = llvm::BasicBlock::Create(_contextGenCode->backend->getContext(), "if", fonctionEnCours);
    llvm::BasicBlock* blocElse = llvm::BasicBlock::Create(_contextGenCode->backend->getContext(), "else");
    llvm::BasicBlock* blocFin = llvm::BasicBlock::Create(_contextGenCode->backend->getContext(), "endif");

    // Générer le code de branchement conditionnel c'est la condition br qui jump au flag 
    _contextGenCode->backend->getBuilder().CreateCondBr(cmp, blocThen, blocElse);

    // Générer le code pour le bloc "if"
    _contextGenCode->backend->getBuilder().SetInsertPoint(blocThen);

}
