#include "Compilateur/AST/Noeuds/Variable/NoeudDeclaration.h"
#include "Compilateur/LLVM/LLVMBackend.h"

#include <llvm-18/llvm/IR/Instructions.h>
#include <llvm-18/llvm/IR/Value.h>
#include <memory>

NoeudDeclaration::NoeudDeclaration(std::shared_ptr<LLVMBackend> backend, const std::string& nom, llvm::Type* type, llvm::Value* valeur)
    : _backend(backend), _nom(nom), _type(type), _arraySize(nullptr), _valeur(valeur)
{
}

NoeudDeclaration::~NoeudDeclaration()
{
}

llvm::Value* NoeudDeclaration::genCode()
{
    llvm::AllocaInst* allocaInst = allocation();
    initialisation(allocaInst);
    return allocaInst;
}

llvm::AllocaInst* NoeudDeclaration::allocation()
{
    return _backend->getBuilder().CreateAlloca(_type, _arraySize, _nom);
}

llvm::AllocaInst* NoeudDeclaration::initialisation(llvm::AllocaInst* allocaInst)
{
    if (_valeur != nullptr && allocaInst != nullptr)
    {
        _backend->getBuilder().CreateStore(_valeur, allocaInst);
    }
    return allocaInst;
}
