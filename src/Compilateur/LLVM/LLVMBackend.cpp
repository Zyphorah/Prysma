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

void LLVMBackend::print(llvm::Value* valeur) {

    // Déclaration de la fonction printf
    llvm::FunctionType *printfType = llvm::FunctionType::get(
        builder->getInt32Ty(),
        {llvm::PointerType::getUnqual(llvm::Type::getInt8Ty(*context))},
        true  // variadic
    );
    llvm::FunctionCallee printfFunc = module->getOrInsertFunction("printf", printfType);
    
    // Format string "Résultat: %f\n"
    std::string formatStr = "Resultat: %f\n";
    llvm::Constant *formatConst = llvm::ConstantDataArray::getString(*context, formatStr);
    llvm::GlobalVariable *formatGlobal = new llvm::GlobalVariable(
        *module,
        formatConst->getType(),
        true,  // isConstant
        llvm::GlobalValue::PrivateLinkage,
        formatConst,
        ".str"
    );
    
    llvm::Value *formatPtr = builder->CreateGEP(
        formatGlobal->getValueType(),
        formatGlobal,
        {builder->getInt64(0), builder->getInt64(0)},
        "formatPtr"
    );
    
    // Appel à printf avec le résultat
    std::vector<llvm::Value*> printfArgs = {formatPtr, valeur};
    builder->CreateCall(printfFunc, printfArgs);
}

void LLVMBackend::creationFonctionMain() {
    // ===== Création de la fonction main LLVM =====
    llvm::FunctionType *funcType = llvm::FunctionType::get(builder->getInt32Ty(), false);
    llvm::Function *mainFunc = llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "main", module.get());
    llvm::BasicBlock *entry = llvm::BasicBlock::Create(*context, "entry", mainFunc);
    builder->SetInsertPoint(entry);
}