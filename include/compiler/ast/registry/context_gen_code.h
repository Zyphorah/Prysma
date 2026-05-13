//===-- context_gen_code.h --------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#ifndef C2537ED8_1CCF_4242_BDB0_B5ED5F2AD08F
#define C2537ED8_1CCF_4242_BDB0_B5ED5F2AD08F

#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include "compiler/ast/registry/registry_type.h"
#include "registry_instruction.h"
#include "stack/registry_variable.h"
#include "registry_function.h"
#include "registry_type.h"
#include "registry_argument.h"
#include "registry_class.h"
#include "stack/return_context_compilation.h"
#include "../../llvm/llvm_backend.h"
#include "llvm/Support/Allocator.h"

struct ContextGenCode
{
private:
    RegistryType* _registryType;
    Symbol _temporaryValue;
    LlvmBackend* _backend;
    RegistryInstruction* _registryInstruction;
    RegistryVariable* _registryVariable;
    RegistryFunctionGlobal* _registryFunctionGlobal;
    MaterializedFunctionRegistry* _registryFunctionLocal;
    RegistryClassGlobal* _registryClassGlobal;
    ReturnContextCompilation* _returnContextCompilation;
    RegistryArgument* _registryArgument;
    llvm::BumpPtrAllocator* _arena;
    std::string _currentFilePath;
    std::string _currentClassName;

public:
    ContextGenCode(
        RegistryType* registryType,
        LlvmBackend* backend,
        RegistryInstruction* registryInstruction,
        RegistryVariable* registryVariable,
        RegistryFunctionGlobal* registryFunctionGlobal,
        MaterializedFunctionRegistry* registryFunctionLocal,
        RegistryClassGlobal* registryClassGlobal,
        ReturnContextCompilation* returnContextCompilation,
        RegistryArgument* registryArgument,
        Symbol temporaryValue,
        llvm::BumpPtrAllocator* arena,
        std::string currentFilePath
    ) 
    {
        try {
            if (currentFilePath.empty()) {
                throw std::invalid_argument("The current file path cannot be empty");
            }
            if (registryType == nullptr)
            {
                throw std::invalid_argument("The file registry cannot be null");
            }
            if (backend == nullptr) {
                throw std::invalid_argument("The LLVM backend cannot be null");
            }
            if (registryInstruction == nullptr) {
                throw std::invalid_argument("The instruction registry cannot be null");
            }
            if (registryVariable == nullptr) {
                throw std::invalid_argument("The variable registry cannot be null");
            }
            if (registryFunctionGlobal == nullptr) {
                throw std::invalid_argument("The global function registry cannot be null");
            }
            if (registryFunctionLocal == nullptr) {
                throw std::invalid_argument("The local function registry cannot be null");
            }
            if (registryClassGlobal == nullptr) {
                throw std::invalid_argument("The global class registry cannot be null");
            }
            if (registryType == nullptr) {
                throw std::invalid_argument("The type registry cannot be null");
            }
            if (returnContextCompilation == nullptr) {
                throw std::invalid_argument("The return context compilation cannot be null");
            }
            if (registryArgument == nullptr) {
                throw std::invalid_argument("The argument registry cannot be null");
            }
            if (arena == nullptr) {
                throw std::invalid_argument("The arena cannot be null");
            }
        } catch (const std::invalid_argument& e) {
            std::cerr << "Error during code generation context creation: " << e.what() << "\n";
            throw;
        }
        _registryType = registryType;
        _currentFilePath = std::move(currentFilePath);
        _backend = backend;
        _registryInstruction = registryInstruction;
        _registryVariable = registryVariable;
        _registryFunctionGlobal = registryFunctionGlobal;
        _registryFunctionLocal = registryFunctionLocal;
        _registryClassGlobal = registryClassGlobal;
        _returnContextCompilation = returnContextCompilation;
        _registryArgument = registryArgument;
        _temporaryValue = temporaryValue;
        _arena = arena;
    }

    void setTemporaryValue(Symbol temporaryValue) { _temporaryValue = temporaryValue; }
    void setCurrentClassName(std::string currentClassName) { _currentClassName = std::move(currentClassName); }

    // Getters
    [[nodiscard]] auto getRegistryType() const -> RegistryType* { return _registryType; }
    [[nodiscard]] auto getTemporaryValue() const -> Symbol { return _temporaryValue; }
    [[nodiscard]] auto getBackend() const -> LlvmBackend* { return _backend; }
    [[nodiscard]] auto getRegistryInstruction() const -> RegistryInstruction* { return _registryInstruction; }
    [[nodiscard]] auto getRegistryVariable() const -> RegistryVariable* { return _registryVariable; }
    [[nodiscard]] auto getRegistryFunctionGlobal() const -> RegistryFunctionGlobal* { return _registryFunctionGlobal; }
    [[nodiscard]] auto getMaterializedFunctionRegistry() const -> MaterializedFunctionRegistry* { return _registryFunctionLocal; }
    [[nodiscard]] auto getRegistryClassGlobal() const -> RegistryClassGlobal* { return _registryClassGlobal; }
    [[nodiscard]] auto getReturnContextCompilation() const -> ReturnContextCompilation* { return _returnContextCompilation; }
    [[nodiscard]] auto getRegistryArgument() const -> RegistryArgument* { return _registryArgument; }
    [[nodiscard]] auto getArena() const -> llvm::BumpPtrAllocator* { return _arena; }
    [[nodiscard]] auto getCurrentFilePath() const -> const std::string& { return _currentFilePath; }
    [[nodiscard]] auto getCurrentClassName() const -> const std::string& { return _currentClassName; }
};

#endif /* C2537ED8_1CCF_4242_BDB0_B5ED5F2AD08F */
