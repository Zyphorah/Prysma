//===-- llvm_backend.cpp -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#include "compiler/llvm/llvm_backend.h"
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/InstrTypes.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/NoFolder.h>
#include <llvm/IR/Value.h>
#include <llvm/Support/Casting.h>
#include <llvm/Support/CodeGen.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/TargetParser/Host.h>
#include <memory>
#include <string>
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

llvm::Value* LlvmBackend::createAutoCast(llvm::Value* sourceValue, llvm::Type* targetType)
{
    if (sourceValue->getType() == targetType)
    {
        return sourceValue;
    }
    // Check if it's an array
    if(targetType->isArrayTy() && sourceValue->getType()->isPointerTy())
    {
        llvm::Value* zero = _builder->getInt32(0);
        std::vector<llvm::Value*> indices = { zero, zero };

        return _builder->CreateInBoundsGEP(
            targetType,      
            sourceValue, 
            indices,       
            "autocast_array_to_ptr"
        );
    }

    llvm::Instruction::CastOps opcode = llvm::CastInst::getCastOpcode(
        sourceValue, 
        true,       
        targetType, 
        true      
    );

    return _builder->CreateCast(opcode, sourceValue, targetType, "autocast");
}

void LlvmBackend::declareExternal(const std::string& name, llvm::Type* ret, const std::vector<llvm::Type*>& args)
{
    llvm::FunctionType* type = llvm::FunctionType::get(ret, args, false);
    llvm::Function::Create(type, llvm::Function::ExternalLinkage, name, *_module);
}

auto LlvmBackend::loadValue(llvm::Value* memoryAddress, const std::string& variableName) -> llvm::Value*
{
    if (memoryAddress == nullptr) 
    { 
        return nullptr;
    }

    if (auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(memoryAddress)) {
        
        llvm::Type* storedType = allocaInst->getAllocatedType();

        return _builder->CreateLoad(storedType, allocaInst, variableName);
    }
    return memoryAddress;
}

// Create allocation at the beginning of the entry block (but after other allocas): simple position manager, moves the LLVM cursor
void LlvmBackend::setInsertionPointAfterAllocation()
{
    llvm::BasicBlock* insertBlock = _builder->GetInsertBlock();
    llvm::Instruction* insertionPosition = nullptr;
    
    if (insertBlock != nullptr) {
        for (auto& instruction : *insertBlock) {
            if (llvm::dyn_cast<llvm::AllocaInst>(&instruction) != nullptr) {
                insertionPosition = &instruction;
            }
        }
        
        if (insertionPosition != nullptr) {
            llvm::Instruction* nextNode = insertionPosition->getNextNode();
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
