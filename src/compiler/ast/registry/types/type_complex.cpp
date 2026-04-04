//===-- type_complex.cpp -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#include "compiler/ast/registry/types/type_complex.h"
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Type.h>
#include <string>

TypeComplex::TypeComplex(const std::string& className) : className(className) {}

auto TypeComplex::generateLLVMType(llvm::LLVMContext& context) -> llvm::Type* {
    return llvm::PointerType::getUnqual(context);
}

auto TypeComplex::isFloating() const -> bool { return false; }
auto TypeComplex::isBoolean() const -> bool { return false; }
auto TypeComplex::isString() const -> bool { return false; }
auto TypeComplex::isArray() const -> bool { return false; }
auto TypeComplex::isComplex() const -> bool { return true; }

auto TypeComplex::getVTableType(llvm::LLVMContext& context) -> llvm::Type* {
    return llvm::PointerType::getUnqual(context);
}

auto TypeComplex::getClassName() const -> std::string {
    return className;
}
