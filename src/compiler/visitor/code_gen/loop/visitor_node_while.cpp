#include "compiler/ast/ast_genere.h"
#include "compiler/ast/nodes/interfaces/i_node.h"
#include "compiler/ast/registry/node_component_registry.h"
#include "compiler/visitor/code_gen/visitor_general_gen_code.h"
#include "compiler/visitor/code_gen/helper/control_flow_helper.h"
#include <llvm-18/llvm/IR/Function.h>
#include <llvm/IR/Value.h>

void GeneralVisitorGenCode::visiter(NodeWhile* nodeWhile) 
{
    auto& nodeData = _contextGenCode->getNodeComponentRegistry()->get<NodeWhileComponents>(
        nodeWhile->getNodeId()
    );

    // Retrieve the condition and blocks from the while node
    INode* nodeCondition = nodeData.getNodeCondition();
    INode* nodeWhileBlock = nodeData.getNodeWhileBlock();
    INode* nodeEndWhileBlock = nodeData.getNodeWhileEndBlock();
    
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