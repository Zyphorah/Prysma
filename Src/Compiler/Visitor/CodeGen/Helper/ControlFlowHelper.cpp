#include "Compiler/Visitor/CodeGen/Helper/ControlFlowHelper.h"
#include <llvm-18/llvm/IR/BasicBlock.h>
#include <string>

auto ControlFlowHelper::createControlBlocks(
    llvm::Function* function,
    llvm::LLVMContext& context,
    const std::string& inputName,
    const std::string& bodyName,
    const std::string& outputName
) -> ControlBlocks {
    return {
        llvm::BasicBlock::Create(context, inputName, function),
        llvm::BasicBlock::Create(context, bodyName, function),
        llvm::BasicBlock::Create(context, outputName, function)
    };
}
