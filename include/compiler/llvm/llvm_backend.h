//===-- llvm_backend.h -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#pragma once

#include <llvm/IR/Value.h>
#include <memory>
#include <string>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/NoFolder.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Constants.h>
#include <llvm/Target/TargetMachine.h>
#include <vector>

class LlvmBackend {
private:
    std::unique_ptr<llvm::LLVMContext> _context;
    std::unique_ptr<llvm::Module> _module;
    std::unique_ptr<llvm::IRBuilder<llvm::NoFolder>> _builder;
    std::unique_ptr<llvm::TargetMachine> _targetMachine;

public:

    LlvmBackend();

    LlvmBackend(const LlvmBackend&) = delete;
    auto operator=(const LlvmBackend&) -> LlvmBackend& = delete;
    LlvmBackend(LlvmBackend&&) = delete;
    auto operator=(LlvmBackend&&) -> LlvmBackend& = delete;

    ~LlvmBackend() = default;

    auto getContext() -> llvm::LLVMContext& { return *_context; }
    auto getModule() -> llvm::Module& { return *_module; }
    auto getBuilder() -> llvm::IRBuilder<llvm::NoFolder>& { return *_builder; }

    auto createAutoCast(llvm::Value* sourceValue, llvm::Type* targetType) -> llvm::Value*;
    void declareExternal(const std::string& name, llvm::Type* ret, const std::vector<llvm::Type*>& args);
    auto loadValue(llvm::Value* memoryAddress, const std::string& variableName) -> llvm::Value*;
    void setInsertionPointAfterAllocation();
};

