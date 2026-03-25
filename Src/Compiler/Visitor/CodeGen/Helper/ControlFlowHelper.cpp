#include "Compiler/Visitor/CodeGen/Helper/ControlFlowHelper.h"
#include <llvm-18/llvm/IR/BasicBlock.h>
#include <string>

auto ControlFlowHelper::creerBlocsControle(
    llvm::Function* function,
    llvm::LLVMContext& context,
    const std::string& nomInput,
    const std::string& nomBody,
    const std::string& nomOutput
) -> BlocsControle {
    return {
        llvm::BasicBlock::Create(context, nomInput, function),
        llvm::BasicBlock::Create(context, nomBody, function),
        llvm::BasicBlock::Create(context, nomOutput, function)
    };
}
