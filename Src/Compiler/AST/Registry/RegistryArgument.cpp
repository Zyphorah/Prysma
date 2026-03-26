#include "Compiler/AST/Registry/RegistryArgument.h"
#include <llvm-18/llvm/IR/Value.h>
#include <vector>

RegistryArgument::RegistryArgument()
= default;

void RegistryArgument::add(llvm::Value* arg)
{
    _args.push_back(arg);
}

auto RegistryArgument::get() -> std::vector<llvm::Value*>&
{
    return _args;
}

void RegistryArgument::clear()
{
    _args.clear();
}



