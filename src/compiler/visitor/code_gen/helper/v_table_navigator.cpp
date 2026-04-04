//===-- v_table_navigator.cpp -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#include "compiler/visitor/code_gen/helper/v_table_navigator.h"
#include <cstdint>
#include <llvm-18/llvm/IR/IRBuilder.h>
#include <llvm-18/llvm/IR/Value.h>
#include <vector>

VTableNavigator::VTableNavigator(llvm::IRBuilderBase* p_builder) : builder(p_builder) {}

auto VTableNavigator::getMethodPointer(llvm::Value* vtable, llvm::Type* vtableType, int methodIndex) -> llvm::Value* {
    llvm::Value* indexValue = builder->getInt32(static_cast<uint32_t>(methodIndex));
    std::vector<llvm::Value*> indices;
    indices.push_back(builder->getInt32(0));
    indices.push_back(indexValue);
    
    llvm::Value* gepPointer = builder->CreateInBoundsGEP(vtableType, vtable, indices, "vtable_gep");
    return builder->CreateLoad(builder->getPtrTy(), gepPointer, "method_ptr");
}
