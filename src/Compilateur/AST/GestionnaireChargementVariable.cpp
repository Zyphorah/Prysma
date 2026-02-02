#include "Compilateur/AST/GestionnaireChargementVariable.h"
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Value.h>

GestionnaireChargementVariable::GestionnaireChargementVariable(std::shared_ptr<LLVMBackend> backend)
    : _backend(std::move(backend))
{
}

llvm::Type* GestionnaireChargementVariable::determinerType(llvm::Value* adresseMemoire)
{
    if (auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(adresseMemoire)) {
        llvm::Type* typeAlloue = allocaInst->getAllocatedType();
        return typeAlloue;
    }
    throw std::runtime_error("Erreur Interne : Impossible de déterminer le type de la variable à l'adresse fournie.");
    return nullptr;
}

llvm::Value* GestionnaireChargementVariable::chargerVariable(llvm::Value* adresseMemoire, const std::string& nomVariable)
{
    llvm::Type* typeVariable = determinerType(adresseMemoire);
    return _backend->getBuilder().CreateLoad(typeVariable, adresseMemoire, nomVariable);
}

void LLVMBackend::creationFonctionMain() {
    // ===== Création de la fonction main LLVM =====
    llvm::FunctionType *funcType = llvm::FunctionType::get(builder->getInt32Ty(), false);
    llvm::Function *mainFunc = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "main", module.get());
    llvm::BasicBlock *entry = llvm::BasicBlock::Create(*context, "entry", mainFunc);
    builder->SetInsertPoint(entry);
}