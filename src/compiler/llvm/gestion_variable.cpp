//===-- gestion_variable.cpp -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#include "compiler/llvm/gestion_variable.h"
#include "compiler/ast/registry/context_gen_code.h"
#include "compiler/ast/registry/stack/registry_variable.h"
#include "compiler/lexer/lexer.h"
#include <llvm-18/llvm/IR/Type.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/Casting.h>
#include <stdexcept>
#include <llvm/Support/FormatVariadic.h>
#include <string>
#include <vector>

VariableTypeExtractor::VariableTypeExtractor(ContextGenCode* context) : _context(context) {}

auto VariableTypeExtractor::extract(llvm::Value* memoryAddress) -> llvm::Type* {
    if (auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(memoryAddress)) {
        return allocaInst->getAllocatedType();
    }
    return llvm::Type::getFloatTy(_context->getBackend()->getContext());
}

VariableAddressor::VariableAddressor(ContextGenCode* context) : _context(context) {}

auto VariableAddressor::getAddress(llvm::StringRef variableName) -> Symbol {
    Token searchToken;
    searchToken.value = variableName;

    if (_context->getRegistryVariable()->variableExists(std::string(variableName))) {
        return _context->getRegistryVariable()->getVariable(searchToken);
    }

    if (!_context->getCurrentClassName().empty()) {
        auto const& classInfo = _context->getRegistryClass()->get(_context->getCurrentClassName());
        if (classInfo->getMemberIndices().find(std::string(variableName)) != classInfo->getMemberIndices().end()) {
            unsigned int idx = classInfo->getMemberIndices().at(std::string(variableName));
            
            Symbol modelSymbol;
            if(classInfo->getRegistryVariable()->variableExists(std::string(variableName))) {
                modelSymbol = classInfo->getRegistryVariable()->getVariable(searchToken);
            }

            Token thisToken; thisToken.value = "this";
            Symbol thisSymbol = _context->getRegistryVariable()->getVariable(thisToken);
            
            llvm::Value* thisPtrAddr = thisSymbol.getAddress();
            llvm::Type* pointerType = llvm::PointerType::getUnqual(_context->getBackend()->getContext());

            llvm::Value* thisPtr = _context->getBackend()->getBuilder().CreateLoad(pointerType, thisPtrAddr, "this_val");

            std::vector<llvm::Value*> indices = {
                _context->getBackend()->getBuilder().getInt32(0),
                _context->getBackend()->getBuilder().getInt32(idx)
            };

            llvm::Value* fieldPtr = _context->getBackend()->getBuilder().CreateGEP(
                classInfo->getStructType(),
                thisPtr,
                indices,
                variableName + "_ptr"
            );

            return {fieldPtr, modelSymbol.getType(), modelSymbol.getPointedElementType()};
        }
    }

    throw std::runtime_error(llvm::formatv("Semantic error: Variable '{0}' is not declared.", variableName).str());
}

VariableLoader::VariableLoader(ContextGenCode* context) 
    : _context(context), _addressor(context), _extractor(context) {}

Symbol VariableLoader::load(llvm::StringRef variableName) {
    Symbol symbol = _addressor.getAddress(variableName);
    llvm::Value* memoryAddress = symbol.getAddress();
    
    llvm::Type* valueType = nullptr;
    if (symbol.getType() != nullptr) {
        valueType = symbol.getType()->generateLLVMType(_context->getBackend()->getContext());
    } else {
        valueType = _extractor.extract(memoryAddress);
    }
    
    llvm::Value* loadedValue = _context->getBackend()->getBuilder().CreateLoad(
        valueType,
        memoryAddress,
        variableName
    );

    symbol = Symbol(loadedValue, symbol.getType(), symbol.getPointedElementType());
    return symbol;
}

auto VariableLoader::loadUnref(llvm::StringRef variableName) -> Symbol {
    Symbol symbol = _addressor.getAddress(variableName);
    llvm::Value* memoryAddress = symbol.getAddress();
    
    llvm::Type* valueType = nullptr;
    if (symbol.getType() != nullptr) {
        valueType = symbol.getType()->generateLLVMType(_context->getBackend()->getContext());
    } else {
        valueType = _extractor.extract(memoryAddress);
    }
    
    if (memoryAddress == nullptr) {
        throw std::runtime_error(llvm::formatv("Semantic error: Variable '{0}' is not declared.", variableName).str());
    }

    if(valueType->isPointerTy()) {   
        llvm::Value* pointerAddress = _context->getBackend()->getBuilder().CreateLoad(
            valueType,
            memoryAddress,
            variableName
        );

        llvm::Type* pointedElementType = symbol.getPointedElementType();
        if (pointedElementType == nullptr) {
            pointedElementType = llvm::Type::getInt32Ty(_context->getBackend()->getContext());
        }

        llvm::Value* loadedSecondJump = _context->getBackend()->getBuilder().CreateLoad(
            pointedElementType,
            pointerAddress,
            variableName + "_unref"
        );

        symbol = Symbol(loadedSecondJump, symbol.getType(), symbol.getPointedElementType());
    } else {
        throw std::runtime_error(llvm::formatv("Semantic error: Variable '{0}' is not a pointer. Unref can only be applied to pointers.", variableName).str());
    }

    return symbol;
}

VariableAllocator::VariableAllocator(ContextGenCode* context) : _context(context) {}

llvm::AllocaInst* VariableAllocator::allocate(llvm::Type* type, llvm::StringRef variableName) {
    llvm::BasicBlock* insertBlock = _context->getBackend()->getBuilder().GetInsertBlock();
    _context->getBackend()->setInsertionPointAfterAllocation();

    llvm::AllocaInst* allocaInst = _context->getBackend()->getBuilder().CreateAlloca(type, nullptr, variableName);
    
    _context->getBackend()->getBuilder().SetInsertPoint(insertBlock);

    return allocaInst;
}

void VariableAllocator::store(llvm::Value* value, llvm::AllocaInst* allocaInst) {
    _context->getBackend()->getBuilder().CreateStore(value, allocaInst);
}

VariableAssigner::VariableAssigner(ContextGenCode* context) 
    : _context(context), _extractor(context) {}

void VariableAssigner::assign(llvm::Value* existingVariable, llvm::Value* value, llvm::Type* llvmVariableType) {
    llvm::Type* typeToAllocate = llvmVariableType;
    if (typeToAllocate == nullptr) {
        if (auto* allocaValue = llvm::dyn_cast<llvm::AllocaInst>(existingVariable)) {
            typeToAllocate = allocaValue->getAllocatedType();
        } else {
            typeToAllocate = _extractor.extract(existingVariable);
        }
    }

    if(typeToAllocate->isPointerTy()) {
        llvm::Value* loadedValue = _context->getBackend()->getBuilder().CreateLoad(
            _context->getBackend()->getBuilder().getPtrTy(),
            existingVariable,
            "loaded_ptr"
        );
        _context->getBackend()->getBuilder().CreateStore(value, loadedValue);
    } else {
        llvm::Value* castedValue = _context->getBackend()->createAutoCast(value, typeToAllocate);
        _context->getBackend()->getBuilder().CreateStore(castedValue, existingVariable);
    }
}
