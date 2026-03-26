#ifndef LLVMSERIALIZER_H
#define LLVMSERIALIZER_H

#include <llvm-18/llvm/Support/raw_ostream.h>
#include "llvm/IR/Module.h"
#include "llvm/IR/LLVMContext.h"
#include <string>
#include <system_error>

using namespace llvm;
using namespace std;

class LlvmSerializer {
    private: 
        Module* _module;
        error_code errorCode;
            
    public: 
        explicit LlvmSerializer(Module& mod) : _module(&mod) {}
        void SaveLLVMCode(const string& path);
};

#endif /* LLVMSERIALIZER_H */