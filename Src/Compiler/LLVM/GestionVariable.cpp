#include "Compiler/LLVM/GestionVariable.h"
#include "Compiler/AST/Registry/ContextGenCode.h"
#include "Compiler/AST/Registry/Stack/RegistryVariable.h"
#include "Compiler/Lexer/Lexer.h"
#include <llvm-18/llvm/IR/Type.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/Casting.h>
#include <stdexcept>
#include <string>
#include <vector>

VariableTypeExtractor::VariableTypeExtractor(ContextGenCode* context) : _context(context) {}

llvm::Type* VariableTypeExtractor::extract(llvm::Value* memoryAddress) {
    if (auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(memoryAddress)) {
        return allocaInst->getAllocatedType();
    }
    return llvm::Type::getFloatTy(_context->getBackend()->getContext());
}

VariableAddressor::VariableAddressor(ContextGenCode* context) : _context(context) {}

Symbol VariableAddressor::getAddress(const std::string& variableName) {
    Token searchToken;
    searchToken.value = variableName;

    if (_context->getRegistryVariable()->variableExists(variableName)) {
        return _context->getRegistryVariable()->getVariable(searchToken);
    }

    if (!_context->getCurrentClassName().empty()) {
        auto const& classInfo = _context->getRegistryClass()->get(_context->getCurrentClassName());
        if (classInfo->getMemberIndices().find(variableName) != classInfo->getMemberIndices().end()) {
            unsigned int idx = classInfo->getMemberIndices().at(variableName);
            
            Symbol modelSymbol;
            if(classInfo->getRegistryVariable()->variableExists(variableName)) {
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

            return Symbol(fieldPtr, modelSymbol.getType(), modelSymbol.getPointedElementType());
        }
    }

    throw std::runtime_error("Semantic error: Variable '" + variableName + "' is not declared.");
}

VariableLoader::VariableLoader(ContextGenCode* context) 
    : _context(context), _addressor(context), _extractor(context) {}

Symbol VariableLoader::load(const std::string& variableName) {
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

Symbol VariableLoader::loadUnref(const std::string& variableName) {
    Symbol symbol = _addressor.getAddress(variableName);
    llvm::Value* memoryAddress = symbol.getAddress();
    
    llvm::Type* valueType = nullptr;
    if (symbol.getType() != nullptr) {
        valueType = symbol.getType()->generateLLVMType(_context->getBackend()->getContext());
    } else {
        valueType = _extractor.extract(memoryAddress);
    }
    
    if (memoryAddress == nullptr) {
        throw std::runtime_error("Semantic error: Variable '" + variableName + "' is not declared.");
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
        throw std::runtime_error("Semantic error: Variable '" + variableName + "' is not a pointer. Unref can only be applied to pointers.");
    }

    return symbol;
}

VariableAllocator::VariableAllocator(ContextGenCode* context) : _context(context) {}

llvm::AllocaInst* VariableAllocator::allocate(llvm::Type* type, const std::string& variableName) {
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
