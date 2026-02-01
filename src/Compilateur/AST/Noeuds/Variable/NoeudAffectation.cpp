#include "Compilateur/AST/Noeuds/Variable/NoeudAffectation.h"
#include "Compilateur/LLVM/LLVMBackend.h"

#include <llvm/IR/Instructions.h>
#include <llvm/IR/Value.h>
#include <memory>
#include <stdexcept>

NoeudAffectation::NoeudAffectation(std::shared_ptr<LLVMBackend> backend, const std::string& nom, llvm::AllocaInst* variableAssigner, std::shared_ptr<INoeud> expression)
    : _backend(backend), _nom(nom), _variableAssigner(variableAssigner), _expression(expression)
{
}

NoeudAffectation::~NoeudAffectation()
{
}

llvm::Value* NoeudAffectation::genCode()
{
    // Évaluer l'expression et assigner immédiatement
    llvm::Value* valeur = nullptr;
    if (_expression != nullptr) {
        valeur = _expression->genCode();
    }
    assignation(_variableAssigner, valeur);
    
    return nullptr;
}

void NoeudAffectation::assignation(llvm::AllocaInst* allocaInst, llvm::Value* valeur)
{
    if (valeur != nullptr && allocaInst != nullptr)
    {
        // Si la valeur est une AllocaInst (variable), charger sa valeur
        if (auto* sourceAlloca = llvm::dyn_cast<llvm::AllocaInst>(valeur)) {
            llvm::Value* loadedValue = _backend->getBuilder().CreateLoad(sourceAlloca->getAllocatedType(), sourceAlloca);
            _backend->getBuilder().CreateStore(loadedValue, allocaInst);
        } else {
            // Sinon, assigner directement la valeur
            _backend->getBuilder().CreateStore(valeur, allocaInst);
        }
    }
}
