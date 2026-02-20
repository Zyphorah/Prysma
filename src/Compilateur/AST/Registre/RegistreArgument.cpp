#include "Compilateur/AST/Registre/RegistreArgument.h"

RegistreArgument::RegistreArgument()
{
}

void RegistreArgument::ajouter(llvm::Value* arg)
{
    _vecArg.push_back(arg);
}

std::vector<llvm::Value*>& RegistreArgument::recuperer()
{
    return _vecArg;
}

void RegistreArgument::vider()
{
    _vecArg.clear();
}



