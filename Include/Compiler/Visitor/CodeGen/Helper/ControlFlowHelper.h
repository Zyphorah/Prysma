#ifndef C6E13746_23A9_489F_AD56_E4937A64E42F
#define C6E13746_23A9_489F_AD56_E4937A64E42F

#include <llvm-18/llvm/IR/BasicBlock.h>
#include <llvm-18/llvm/IR/Function.h>
#include <llvm-18/llvm/IR/LLVMContext.h>
#include <string>

struct BlocsControle {
    llvm::BasicBlock* input;
    llvm::BasicBlock* body; 
    llvm::BasicBlock* output;
};

class ControlFlowHelper {
public:
    static auto creerBlocsControle(
        llvm::Function* function,
        llvm::LLVMContext& context,
        const std::string& nomInput,
        const std::string& nomBody, 
        const std::string& nomOutput
    ) -> BlocsControle;
};

#endif /* C6E13746_23A9_489F_AD56_E4937A64E42F */
