//===-- llvm_methods.cpp -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

// Inclusions LLVM
#include "compiler/ast/registry/context_gen_code.h"
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Support/raw_ostream.h>

#include <llvm/IR/Type.h>
#include <llvm/IR/Constants.h>  
#include <llvm/IR/Instructions.h>
#include "compiler/llvm/llvm_serializer.h"

#include <llvm/TargetParser/Host.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>

using namespace llvm;

// Commande de compilation de l'exemple, utilisé pour mieu comprendre le framework LLVM
//g++ -fdiagnostics-color=always -g -O0 -std=c++17 -I../include LLVMMethodeDoc.cpp ../src/Compiler/LLVM/LLVMSerializer.cpp $(llvm-config --cxxflags --ldflags --libs) -o LLVMMethodeDoc

// ===== Initialization LLVM =====
llvm::LLVMContext context;
llvm::Module module("output", context);
llvm::IRBuilder<> builder(context);

AllocaInst* exempleInitializationVariable()
{
    // Type pour la variable int 
    Type * typeInt = builder.getInt32Ty();
    Value * ArraySize = nullptr; 
    
    // Création d'un espace mémoire pour la variable X (int32) 
    AllocaInst *allocaX = builder.CreateAlloca(typeInt, ArraySize, "x");

    // Initialization de la valeur dans X (store) soit int x = 10 
    int value = 10;
    Value *valConst = ConstantInt::get(typeInt, value);
    builder.CreateStore(valConst, allocaX);
    return allocaX; 
}

// ===== Exemple d'utilisation : lire, ajouter 5 et réécrire ======
void LireUneValeurEtAjouter5(AllocaInst* allocaX)
{
    Type * typeInt = builder.getInt32Ty();
    const Twine NameTmp = "loadtmp"; 
    
    // On utilise les instructions comme CreateAdd pour créer une instruction assembleur intermédiaire pour effectuer 
    // une addition, multiplication, division, soustraction. 
    Value *loaded = builder.CreateLoad(typeInt, allocaX, NameTmp);
    Value *added = builder.CreateAdd(loaded, ConstantInt::get(typeInt, 5), NameTmp);
    builder.CreateStore(added, allocaX);
}

void sauvegarderCode()
{
    LLVMSerializer traitementFile{context, module};
    traitementFile.SauvegarderCodeLLVM("output.ll");
}

void declarationFunction()
{
    // 1. Signature : void (i32, float)
    std::vector<llvm::Type*> args = { builder.getInt32Ty(), builder.getFloatTy() };
    llvm::FunctionType* ft = llvm::FunctionType::get(builder.getVoidTy(), args, false);

    // 2. Création
    llvm::Function* function = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, "maFunction", module);

    // 3. Création du bloc
    llvm::BasicBlock* blocEntry = llvm::BasicBlock::Create(context, "entry", function);
    builder.SetInsertPoint(blocEntry);

    // 4. Copie des arguments (Boilerplate)
    auto argIt = function->arg_begin();
    llvm::Value* valX = argIt++; // Récupère le 1er arg
    llvm::Value* valY = argIt++; // Récupère le 2ème arg

    // Pour X
    llvm::Value* ptrX = builder.CreateAlloca(builder.getInt32Ty(), nullptr, "ptr_x");
    builder.CreateStore(valX, ptrX);

    // Pour Y
    llvm::Value* ptrY = builder.CreateAlloca(builder.getFloatTy(), nullptr, "ptr_y");
    builder.CreateStore(valY, ptrY);

    // 5. Fin
    builder.CreateRetVoid();
}

int main()
{
    // Configuration de la cible, évite d'avoir des adresses mémoire aléatoires, ce qui cause des crashs aléatoires. 
    InitializeAllTargetInfos();
    InitializeAllTargets();
    InitializeAllTargetMCs();
    InitializeAllAsmPrinters();

    std::string targetTriple = sys::getDefaultTargetTriple();
    module.setTargetTriple(targetTriple);

    std::string error;
    auto target = TargetRegistry::lookupTarget(targetTriple, error);
    if (!target) {
        errs() << error;
        return 1;
    }

    TargetOptions opt;
    auto targetMachine = target->createTargetMachine(targetTriple, "generic", "", opt, Reloc::Model::PIC_);
    
    // Définit le layout mémoire correct (alignement standard et espace 0)
    module.setDataLayout(targetMachine->createDataLayout());
    // ------------------------------------------------------------------

    // Tout doit se trouver dans une function avant l'exécution. 
    // 1. Définir le type de la function (void func())
    FunctionType *ftype = FunctionType::get(Type::getVoidTy(context), false);
    
    // 2. Créer la function "main" dans le module
    Function *mainFunc = Function::Create(ftype, Function::ExternalLinkage, "main", module);
    
    // 3. Créer un BasicBlock nommé "entry" à l'intérieur de cette function
    BasicBlock *bb = BasicBlock::Create(context, "entry", mainFunc);
    
    // 4. Positionner le curseur du builder dans ce bloc (CRUCIAL)
    builder.SetInsertPoint(bb);

    // Maintenant le builder sait où écrire
    AllocaInst* allocatX = exempleInitializationVariable();
    LireUneValeurEtAjouter5(allocatX);
    
    // Toujours terminer un bloc par un return (void ici) pour qu'il soit valide
    builder.CreateRetVoid();

    sauvegarderCode();

    return 0; 
}
