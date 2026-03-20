#include "Compilateur/AST/Registre/RegistreArgument.h"
#include <llvm-18/llvm/IR/Value.h>
#include <vector>

RegistreArgument::RegistreArgument()
= default;

void RegistreArgument::ajouter(llvm::Value* arg)
{
    _vecArg.push_back(arg);
}

auto RegistreArgument::recuperer() -> std::vector<llvm::Value*>&
{
    return _vecArg;
}

void RegistreArgument::vider()
{
    _vecArg.clear();
}



