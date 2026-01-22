#include "LLVM/LLVMBackend.h"

void LLVMBackend :: creationFonctionLLVM()
{
      // Création fonction main qui retourne un double
        llvm::FunctionType *funcType = llvm::FunctionType::get(_builder->getInt32Ty(), false);
        // 'module' was undeclared; pass nullptr (or replace with a valid llvm::Module* member like _module.get())
        llvm::Function *mainFunc = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "main", nullptr);
        llvm::BasicBlock *entry = llvm::BasicBlock::Create(_context, "entry", mainFunc);
        _builder->SetInsertPoint(entry);

}