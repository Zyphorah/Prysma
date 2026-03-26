#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/Visitor/CodeGen/VisitorGeneralGenCode.h"
#include "Compiler/Visitor/CodeGen/Helper/ControlFlowHelper.h"
#include <llvm-18/llvm/IR/Function.h>
#include <llvm/IR/Value.h>

void GeneralVisitorGenCode::visiter(NodeWhile* nodeWhile) 
{
    // Retrieve the condition and blocks from the while node
    INode* nodeCondition = nodeWhile->getNodeCondition();
    INode* nodeWhileBlock = nodeWhile->getNodeBlocWhile();
    INode* nodeEndWhileBlock = nodeWhile->getNodeBlocFinWhile();
    
    // Build the basic blocks for the while
    llvm::Function* currentFunction = _contextGenCode->getBackend()->getBuilder().GetInsertBlock()->getParent();
    auto [conditionBlock, whileBlock, endWhileBlock] = ControlFlowHelper::createControlBlocks(currentFunction, _contextGenCode->getBackend()->getContext(), "while.cond", "while.body", "while.end");

    // Generate code for the while condition while.cond
    _contextGenCode->getBackend()->getBuilder().CreateBr(conditionBlock);
    _contextGenCode->getBackend()->getBuilder().SetInsertPoint(conditionBlock); 
    nodeCondition->accept(this);
    llvm::Value* cmp = _contextGenCode->getTemporaryValue().getAddress();
    _contextGenCode->getBackend()->getBuilder().CreateCondBr(cmp, whileBlock, endWhileBlock);

    // Generate code for the while block while.body
    _contextGenCode->getBackend()->getBuilder().SetInsertPoint(whileBlock);
    if (nodeWhileBlock != nullptr) {
        nodeWhileBlock->accept(this);
        _contextGenCode->getBackend()->getBuilder().CreateBr(conditionBlock);
    }
    _contextGenCode->getBackend()->getBuilder().SetInsertPoint(endWhileBlock);
    if (nodeEndWhileBlock != nullptr) {
        nodeEndWhileBlock->accept(this);
    }
}