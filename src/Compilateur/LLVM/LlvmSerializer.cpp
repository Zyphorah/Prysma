#include "Compilateur/LLVM/LlvmSerializer.h"
#include <llvm-18/llvm/Support/raw_ostream.h>
#include <string>

void LlvmSerializer::SauvegarderCodeLLVM(const string& path)
{
        llvm::raw_fd_ostream out(path, errorCode);
        _module->print(out, nullptr);
        out.close();
}