#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/Visitor/CodeGen/VisitorGeneralGenCode.h"
#include "Compiler/Visitor/CodeGen/Helper/ControlFlowHelper.h"
#include <llvm-18/llvm/IR/Function.h>
#include <llvm/IR/Value.h>

void GeneralVisitorGenCode::visiter(NodeWhile* nodeWhile) 
{
    // Récupérer la condition et les blocs à partir du node while
    INode* nodeCondition = nodeWhile->getNodeCondition();
    INode* nodeBlocWhile = nodeWhile->getNodeBlocWhile();
    INode* nodeBlocEndWhile = nodeWhile->getNodeBlocFinWhile();
    
    // construire les blocs de base pour le while 
    llvm::Function* functionEnCours = _contextGenCode->getBackend()->getBuilder().GetInsertBlock()->getParent();
    auto [blocCondition, blocWhile, blocEndWhile] = ControlFlowHelper::creerBlocsControle(functionEnCours, _contextGenCode->getBackend()->getContext(), "while.cond", "while.body", "while.end");

    // Générer le code pour la condition du while while.cond
    _contextGenCode->getBackend()->getBuilder().CreateBr(blocCondition);
    _contextGenCode->getBackend()->getBuilder().SetInsertPoint(blocCondition); 
    nodeCondition->accept(this);
    llvm::Value* cmp = _contextGenCode->getValeurTemporaire().getAdresse();
    _contextGenCode->getBackend()->getBuilder().CreateCondBr(cmp, blocWhile, blocEndWhile);

    // Générer le code pour le bloc du while while.body

    _contextGenCode->getBackend()->getBuilder().SetInsertPoint(blocWhile);
    if (nodeBlocWhile != nullptr) {
        nodeBlocWhile->accept(this);
        _contextGenCode->getBackend()->getBuilder().CreateBr(blocCondition);

    }
    _contextGenCode->getBackend()->getBuilder().SetInsertPoint(blocEndWhile);
    if (nodeBlocEndWhile != nullptr) {
        nodeBlocEndWhile->accept(this);
    }
}