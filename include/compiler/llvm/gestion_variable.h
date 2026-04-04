//===-- gestion_variable.h -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#ifndef D5B94044_819C_4C15_B528_AAE97CBCB264
#define D5B94044_819C_4C15_B528_AAE97CBCB264

#include "compiler/ast/registry/context_gen_code.h"
#include "compiler/ast/registry/stack/registry_variable.h"
#include <llvm-18/llvm/IR/Instructions.h>
#include <llvm-18/llvm/IR/Value.h>
#include <string>

class VariableTypeExtractor {
private:
    ContextGenCode* _context;
public:
    explicit VariableTypeExtractor(ContextGenCode* context);
    auto extract(llvm::Value* memoryAddress) -> llvm::Type*;
};

class VariableAddressor {
private:
    ContextGenCode* _context;
public:
    explicit VariableAddressor(ContextGenCode* context);
    auto getAddress(llvm::StringRef variableName) -> Symbol;
};

class VariableLoader {
private:
    ContextGenCode* _context;
    VariableAddressor _addressor;
    VariableTypeExtractor _extractor;
public:
    explicit VariableLoader(ContextGenCode* context);
    auto load(llvm::StringRef variableName) -> Symbol;
    auto loadUnref(llvm::StringRef variableName) -> Symbol;
};

class VariableAllocator {
private:
    ContextGenCode* _context;
public:
    explicit VariableAllocator(ContextGenCode* context);
    auto allocate(llvm::Type* type, llvm::StringRef variableName) -> llvm::AllocaInst*;
    void store(llvm::Value* value, llvm::AllocaInst* allocaInst);
};

class VariableAssigner {
private:
    ContextGenCode* _context;
    VariableTypeExtractor _extractor;
public:
    explicit VariableAssigner(ContextGenCode* context);
    void assign(llvm::Value* existingVariable, llvm::Value* value, llvm::Type* llvmVariableType = nullptr);
};

#endif /* D5B94044_819C_4C15_B528_AAE97CBCB264 */
