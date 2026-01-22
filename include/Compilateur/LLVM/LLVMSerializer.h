#include <llvm-18/llvm/Support/raw_ostream.h>
#include "llvm/IR/Module.h"
#include "llvm/IR/LLVMContext.h"
#include <string>

using namespace llvm;
using namespace std;

class LLVMSerializer {
    private: 
        LLVMContext& _context;
        Module& _module;
        error_code errorCode;
            
    public: 
        LLVMSerializer(LLVMContext& ctx, Module& mod) : _context(ctx), _module(mod) {}
        void SauvegarderCodeLLVM(const string& path);
};