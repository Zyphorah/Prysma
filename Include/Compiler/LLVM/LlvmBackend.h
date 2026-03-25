#pragma once

#include <llvm/IR/Value.h>
#include <memory>
#include <string>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/NoFolder.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Constants.h>
#include <llvm/Target/TargetMachine.h>
#include <vector>

class LlvmBackend {
private:
    std::unique_ptr<llvm::LLVMContext> _context;
    std::unique_ptr<llvm::Module> _module;
    std::unique_ptr<llvm::IRBuilder<llvm::NoFolder>> _builder;
    std::unique_ptr<llvm::TargetMachine> _targetMachine;

public:

    LlvmBackend();

    LlvmBackend(const LlvmBackend&) = delete;
    auto operator=(const LlvmBackend&) -> LlvmBackend& = delete;
    LlvmBackend(LlvmBackend&&) = delete;
    auto operator=(LlvmBackend&&) -> LlvmBackend& = delete;

    ~LlvmBackend() = default;

    auto getContext() -> llvm::LLVMContext& { return *_context; }
    auto getModule() -> llvm::Module& { return *_module; }
    auto getBuilder() -> llvm::IRBuilder<llvm::NoFolder>& { return *_builder; }

    auto creerAutoCast(llvm::Value* valeurSource, llvm::Type* typeCible) -> llvm::Value*;
    void declarerExterne(const std::string& nom, llvm::Type* ret, const std::vector<llvm::Type*>& args);
    auto loadrValeur(llvm::Value* adresseMemory, const std::string& nomVariable) -> llvm::Value*;
    void definirPointInsertionApresAllocation();
};

