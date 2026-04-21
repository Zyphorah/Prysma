#include "compiler/ast/ast_genere.h"
#include "compiler/ast/nodes/interfaces/i_node.h"
#include "compiler/ast/registry/node_component_registry.h"
#include "compiler/visitor/code_gen/visitor_general_gen_code.h"
#include "compiler/visitor/code_gen/helper/control_flow_helper.h"
#include <llvm-18/llvm/IR/Value.h>

void GeneralVisitorGenCode::visiter(NodeIf* nodeIf) 
{
    auto& nodeData = _contextGenCode->getNodeComponentRegistry()->get<NodeIfComponents>(
        nodeIf->getNodeId()
    );

    // Retrieve the condition and blocks from the if node
    INode* nodeCondition = nodeData.getNodeCondition();
    INode* nodeIfBlock = nodeData.getNodeBlocIf();
    INode* nodeElseBlock = nodeData.getNodeBlocElse();

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
