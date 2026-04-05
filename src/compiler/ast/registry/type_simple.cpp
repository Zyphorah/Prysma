#include <iostream>
//===-- type_simple.cpp -----------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#include "compiler/ast/registry/types/type_simple.h"
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Type.h>
#include <stdexcept>
#include <string>

TypeSimple::TypeSimple(llvm::Type* llvmType)
    : _typeID(llvmType->getTypeID()), // TODO : poisoned memory dans le llvmType->getTypeID() accès interdit
      _integerBitWidth(llvmType->isIntegerTy() ? llvmType->getIntegerBitWidth() : 0),
      _pointerAddressSpace(llvmType->isPointerTy() ? llvmType->getPointerAddressSpace() : 0)
{
}

TypeSimple::TypeSimple(llvm::Type::TypeID typeID, unsigned int bitWidth, unsigned int addressSpace)
    : _typeID(typeID), _integerBitWidth(bitWidth), _pointerAddressSpace(addressSpace)
{
}

auto TypeSimple::generateLLVMType(llvm::LLVMContext& context) -> llvm::Type*
{
    switch (_typeID) {
        case llvm::Type::VoidTyID:   return llvm::Type::getVoidTy(context);
        case llvm::Type::FloatTyID:  return llvm::Type::getFloatTy(context);
        case llvm::Type::DoubleTyID: return llvm::Type::getDoubleTy(context);
        case llvm::Type::IntegerTyID:
            return llvm::IntegerType::get(context, _integerBitWidth);
        case llvm::Type::PointerTyID:
            return llvm::PointerType::get(context, _pointerAddressSpace);
        default:
            throw std::runtime_error("Unsupported LLVM type for migration between contexts: "
                                      + std::to_string(_typeID));
    }
}

auto TypeSimple::isFloating() const -> bool
{
    return _typeID == llvm::Type::FloatTyID || _typeID == llvm::Type::DoubleTyID;
}

auto TypeSimple::isBoolean() const -> bool
{
    return _typeID == llvm::Type::IntegerTyID && _integerBitWidth == 1;
}

constexpr int STRING_BIT_WIDTH = 8;

auto TypeSimple::isString() const -> bool
{
    return _typeID == llvm::Type::IntegerTyID && _integerBitWidth == STRING_BIT_WIDTH;
}
