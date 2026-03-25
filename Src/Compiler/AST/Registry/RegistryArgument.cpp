#include "Compiler/AST/Registry/RegistryArgument.h"
#include <llvm-18/llvm/IR/Value.h>
#include <vector>

RegistryArgument::RegistryArgument()
= default;

void RegistryArgument::ajouter(llvm::Value* arg)
{
    _vecArg.push_back(arg);
}

auto RegistryArgument::recuperer() -> std::vector<llvm::Value*>&
{
    return _vecArg;
}

void RegistryArgument::vider()
{
    _vecArg.clear();
}



