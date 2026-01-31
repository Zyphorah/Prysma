#include "Compilateur/AST/Noeuds/Variable/NoeudAffectation.h"
#include "Compilateur/LLVM/LLVMBackend.h"

#include <llvm-18/llvm/IR/Instructions.h>
#include <llvm-18/llvm/IR/Value.h>
#include <memory>
#include <stdexcept>

NoeudAffectation::NoeudAffectation(std::shared_ptr<LLVMBackend> backend, const std::string& nom, llvm::AllocaInst* variableAssigner, llvm::Value* valeur)
    : _backend(backend), _nom(nom), _variableAssigner(variableAssigner), _valeur(valeur)
{
}

NoeudAffectation::~NoeudAffectation()
{
}

llvm::Value* NoeudAffectation::genCode()
{
    assignation(_variableAssigner);
    return _variableAssigner;
}

void NoeudAffectation::assignation(llvm::AllocaInst* allocaInst)
{
    if (_valeur != nullptr && allocaInst != nullptr)
    {
        // Si la valeur est une AllocaInst (variable), charger sa valeur
        if (auto* sourceAlloca = llvm::dyn_cast<llvm::AllocaInst>(_valeur)) {
            llvm::Value* loadedValue = _backend->getBuilder().CreateLoad(sourceAlloca->getAllocatedType(), sourceAlloca);
            _backend->getBuilder().CreateStore(loadedValue, allocaInst);
        } else {
            // Sinon, assigner directement la valeur
            _backend->getBuilder().CreateStore(_valeur, allocaInst);
        }
    }
}
