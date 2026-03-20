#ifndef C6E13746_23A9_489F_AD56_E4937A64E42F
#define C6E13746_23A9_489F_AD56_E4937A64E42F

#include <llvm-18/llvm/IR/BasicBlock.h>
#include <llvm-18/llvm/IR/Function.h>
#include <llvm-18/llvm/IR/LLVMContext.h>
#include <string>

struct BlocsControle {
    llvm::BasicBlock* entree;
    llvm::BasicBlock* corps; 
    llvm::BasicBlock* sortie;
};

class ControlFlowHelper {
public:
    static auto creerBlocsControle(
        llvm::Function* fonction,
        llvm::LLVMContext& context,
        const std::string& nomEntree,
        const std::string& nomCorps, 
        const std::string& nomSortie
    ) -> BlocsControle;
};

#endif /* C6E13746_23A9_489F_AD56_E4937A64E42F */
