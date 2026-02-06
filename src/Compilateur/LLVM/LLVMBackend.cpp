#include "Compilateur/LLVM/LLVMBackend.h"
#include "Compilateur/LLVM/LLVMSerializer.h"
#include <llvm/Support/TargetSelect.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/TargetParser/Host.h>
#include <vector>

using namespace llvm;

LLVMBackend::LLVMBackend() {

    _context = std::make_unique<LLVMContext>();
    _module = std::make_unique<Module>("output", *_context);
    _builder = std::make_unique<IRBuilder<NoFolder>>(*_context);

    // Initialisation de la cible (évite d'avoir des adresses mémoire aléatoires)
    InitializeAllTargetInfos();
    InitializeAllTargets();
    InitializeAllTargetMCs();
    InitializeAllAsmPrinters();

    std::string targetTriple = sys::getDefaultTargetTriple();
    _module->setTargetTriple(targetTriple);
    
    std::string error;
    const auto *target = TargetRegistry::lookupTarget(targetTriple, error);
    if (!target) {
        errs() << error;
    }

    TargetOptions opt;
    target->createTargetMachine(targetTriple, "generic", "", opt, Reloc::Model::PIC_);
}



llvm::Value* LLVMBackend::creerAutoCast(llvm::Value* valeurSource, llvm::Type* typeCible)
{
    if (valeurSource->getType() == typeCible) return valeurSource;

    llvm::Instruction::CastOps opcode = llvm::CastInst::getCastOpcode(
        valeurSource, 
        true,       
        typeCible, 
        true      
    );

    return _builder->CreateCast(opcode, valeurSource, typeCible, "autocast");
}


void LLVMBackend::declarerExterne(const std::string& nom, llvm::Type* ret, std::vector<llvm::Type*>& args)
{
    llvm::FunctionType* type = llvm::FunctionType::get(ret, args, false);
    llvm::Function::Create(type, llvm::Function::ExternalLinkage, nom, *_module);
}