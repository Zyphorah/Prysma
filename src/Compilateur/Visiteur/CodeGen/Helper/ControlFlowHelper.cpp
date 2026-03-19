#include "Compilateur/Visiteur/CodeGen/Helper/ControlFlowHelper.h"
#include <llvm-18/llvm/IR/BasicBlock.h>
#include <string>

BlocsControle ControlFlowHelper::creerBlocsControle(
    llvm::Function* fonction,
    llvm::LLVMContext& context,
    const std::string& nomEntree,
    const std::string& nomCorps,
    const std::string& nomSortie
) {
    return {
        llvm::BasicBlock::Create(context, nomEntree, fonction),
        llvm::BasicBlock::Create(context, nomCorps, fonction),
        llvm::BasicBlock::Create(context, nomSortie, fonction)
    };
}
