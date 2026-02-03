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

    context = std::make_unique<LLVMContext>();
    module = std::make_unique<Module>("output", *context);
    builder = std::make_unique<IRBuilder<NoFolder>>(*context);

    // Initialisation de la cible (évite d'avoir des adresses mémoire aléatoires)
    InitializeAllTargetInfos();
    InitializeAllTargets();
    InitializeAllTargetMCs();
    InitializeAllAsmPrinters();

    std::string targetTriple = sys::getDefaultTargetTriple();
    module->setTargetTriple(targetTriple);
    
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

    return nullptr;
}