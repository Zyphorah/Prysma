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
    // 1. Initialisation des pointeurs dans l'ordre correct
    context = std::make_unique<LLVMContext>();
    module = std::make_unique<Module>("PrysmaModule", *context);
    builder = std::make_unique<IRBuilder<NoFolder>>(*context);

    // 2. Initialisation de la cible (évite d'avoir des adresses mémoire aléatoires)
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

void LLVMBackend::sauvegarderCodeLLVM(const std::string& nomFichier) {
    LLVMSerializer serializer(*context, *module);
    serializer.SauvegarderCodeLLVM(nomFichier);
}
