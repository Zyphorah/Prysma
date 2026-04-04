//===-- v_table_navigator.h -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#ifndef VTABLE_NAVIGATOR_H
#define VTABLE_NAVIGATOR_H

#include <llvm-18/llvm/IR/Value.h>
#include <llvm-18/llvm/IR/IRBuilder.h>

class VTableNavigator {
private:
    llvm::IRBuilderBase* builder;

public:
    explicit VTableNavigator(llvm::IRBuilderBase* p_builder);
    auto getMethodPointer(llvm::Value* vtable, llvm::Type* vtableType, int methodIndex) -> llvm::Value*;
};

#endif
