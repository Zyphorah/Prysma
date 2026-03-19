#include "Compilateur/LLVM/LlvmBackend.h"
#include <llvm/Support/TargetSelect.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/TargetParser/Host.h>
#include <vector>

using namespace llvm;

LlvmBackend::LlvmBackend() {

    _context = std::make_unique<LLVMContext>();
    _module = std::make_unique<Module>("output", *_context);
    _builder = std::make_unique<IRBuilder<NoFolder>>(*_context);

    std::string targetTriple = sys::getDefaultTargetTriple();
    _module->setTargetTriple(targetTriple);
    
    std::string error;
    const auto *target = TargetRegistry::lookupTarget(targetTriple, error);
    if (target == nullptr) {
        errs() << error;
    }

    TargetOptions opt;
    _targetMachine.reset(target->createTargetMachine(targetTriple, "generic", "", opt, Reloc::Model::PIC_));
}



llvm::Value* LlvmBackend::creerAutoCast(llvm::Value* valeurSource, llvm::Type* typeCible)
{
    if (valeurSource->getType() == typeCible)
    {
        return valeurSource;
    }
    // Vérifier que c'est un array 
    if(typeCible->isArrayTy() && valeurSource->getType()->isPointerTy())
    {
        llvm::Value* zero = _builder->getInt32(0);
        std::vector<llvm::Value*> indices = { zero, zero };

        return _builder->CreateInBoundsGEP(
            typeCible,      
            valeurSource, 
            indices,       
            "autocast_array_to_ptr"
        );
    }

    llvm::Instruction::CastOps opcode = llvm::CastInst::getCastOpcode(
        valeurSource, 
        true,       
        typeCible, 
        true      
    );

    return _builder->CreateCast(opcode, valeurSource, typeCible, "autocast");
}


void LlvmBackend::declarerExterne(const std::string& nom, llvm::Type* ret, const std::vector<llvm::Type*>& args)
{
    llvm::FunctionType* type = llvm::FunctionType::get(ret, args, false);
    llvm::Function::Create(type, llvm::Function::ExternalLinkage, nom, *_module);
}

llvm::Value* LlvmBackend::chargerValeur(llvm::Value* adresseMemoire, const std::string& nomVariable)
{
    if (adresseMemoire == nullptr) 
    { 
        return nullptr;
    }

    if (auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(adresseMemoire)) {
        
        llvm::Type* typeStocke = allocaInst->getAllocatedType();

        return _builder->CreateLoad(typeStocke, allocaInst, nomVariable);
    }
    return adresseMemoire;
}

// Créer l'allocation au début du bloc d'entrée (mais après les autres allocas) : simple gestionnaire de position il déplace le curseur LLVM
void LlvmBackend::definirPointInsertionApresAllocation()
{
    llvm::BasicBlock* insertBlock = _builder->GetInsertBlock();
    llvm::Instruction* positionInsertion = nullptr;
    
    if (insertBlock != nullptr) {
        for (auto& instruction : *insertBlock) {
            if (llvm::dyn_cast<llvm::AllocaInst>(&instruction) != nullptr) {
                positionInsertion = &instruction;
            }
        }
        
        if (positionInsertion != nullptr) {
            llvm::Instruction* nextNode = positionInsertion->getNextNode();
            if (nextNode != nullptr) {
                _builder->SetInsertPoint(nextNode);
            } else {
                _builder->SetInsertPoint(insertBlock);
            }
        } else {
            if (insertBlock->empty()) {
                _builder->SetInsertPoint(insertBlock);
            } else {
                _builder->SetInsertPoint(&insertBlock->front());
            }
        }
    }
}