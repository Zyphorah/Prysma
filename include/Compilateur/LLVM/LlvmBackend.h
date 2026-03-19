#pragma once

#include <memory>
#include <string>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/NoFolder.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Constants.h>
#include <llvm/Target/TargetMachine.h>

class LlvmBackend {
private:
    std::unique_ptr<llvm::LLVMContext> _context;
    std::unique_ptr<llvm::Module> _module;
    std::unique_ptr<llvm::IRBuilder<llvm::NoFolder>> _builder;
    std::unique_ptr<llvm::TargetMachine> _targetMachine;

public:

    LlvmBackend();

    ~LlvmBackend() = default;

    llvm::LLVMContext& getContext() { return *_context; }
    llvm::Module& getModule() { return *_module; }
    llvm::IRBuilder<llvm::NoFolder>& getBuilder() { return *_builder; }

    llvm::Value* creerAutoCast(llvm::Value* valeurSource, llvm::Type* typeCible);
    void declarerExterne(const std::string& nom, llvm::Type* ret, const std::vector<llvm::Type*>& args);
    llvm::Value* chargerValeur(llvm::Value* adresseMemoire, const std::string& nomVariable);
    void definirPointInsertionApresAllocation();
};

