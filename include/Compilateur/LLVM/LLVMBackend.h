#pragma once

#include <memory>
#include <string>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/NoFolder.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Constants.h>

class LLVMBackend {
private:
    // L'ordre de déclaration est CRUCIAL ici (C++ détruit dans l'ordre inverse)
    std::unique_ptr<llvm::LLVMContext> context;

    std::unique_ptr<llvm::Module> module;
    
    std::unique_ptr<llvm::IRBuilder<llvm::NoFolder>> builder;

public:

    LLVMBackend();


    ~LLVMBackend() = default;

    llvm::LLVMContext& getContext() { return *context; }
    llvm::Module& getModule() { return *module; }
    llvm::IRBuilder<llvm::NoFolder>& getBuilder() { return *builder; }

    void sauvegarderCodeLLVM(const std::string& nomFichier);
    
    void print(llvm::Value* valeur);
    
    void creationFonctionMain();
};
 
