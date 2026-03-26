#include "Compiler/LLVM/LlvmSerializer.h"
#include <llvm-18/llvm/Support/raw_ostream.h>
#include <string>

void LlvmSerializer::SaveLLVMCode(const string& path)
{
        llvm::raw_fd_ostream out(path, errorCode);
        _module->print(out, nullptr);
        out.close();
}