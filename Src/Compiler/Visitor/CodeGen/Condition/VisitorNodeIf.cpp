#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/Visitor/CodeGen/VisitorGeneralGenCode.h"
#include "Compiler/Visitor/CodeGen/Helper/ControlFlowHelper.h"
#include <llvm-18/llvm/IR/Value.h>

void GeneralVisitorGenCode::visiter(NodeIf* nodeIf) 
{
    // Retrieve the condition and blocks from the if node
    INode* nodeCondition = nodeIf->getNodeCondition();
    INode* nodeIfBlock = nodeIf->getNodeBlocIf();
    INode* nodeElseBlock = nodeIf->getNodeBlocElse();

    // Evaluate the condition
    nodeCondition->accept(this);
    llvm::Value* cmp = _contextGenCode->getTemporaryValue().getAddress(); 

    llvm::Function* currentFunction = _contextGenCode->getBackend()->getBuilder().GetInsertBlock()->getParent();

    auto [thenBlock, elseBlock, endBlock] = ControlFlowHelper::createControlBlocks(currentFunction, _contextGenCode->getBackend()->getContext(), "if", "else", "endif");

    // Generate conditional branch code
    _contextGenCode->getBackend()->getBuilder().CreateCondBr(cmp, thenBlock, elseBlock);

    // Generate code for the "if" block
    _contextGenCode->getBackend()->getBuilder().SetInsertPoint(thenBlock);
    if (nodeIfBlock != nullptr) {
        nodeIfBlock->accept(this);
    }
    // Branch to endif at the end of the if block
    _contextGenCode->getBackend()->getBuilder().CreateBr(endBlock);

    // Generate code for the "else" block
    _contextGenCode->getBackend()->getBuilder().SetInsertPoint(elseBlock);
    if (nodeElseBlock != nullptr) {
        nodeElseBlock->accept(this);
    }
    // Branch to endif at the end of the else block
    _contextGenCode->getBackend()->getBuilder().CreateBr(endBlock);

    // Generate code for the "endif" block
    _contextGenCode->getBackend()->getBuilder().SetInsertPoint(endBlock);
    
    // No more node should process this temporary value after the if
    _contextGenCode->setTemporaryValue(Symbol(nullptr, _contextGenCode->getTemporaryValue().getType(), _contextGenCode->getTemporaryValue().getPointedElementType()));

}
