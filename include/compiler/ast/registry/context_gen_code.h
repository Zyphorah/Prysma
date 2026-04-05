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
    RegistryType* registryType;
    Symbol temporaryValue;
    LlvmBackend* backend;
    RegistryInstruction* registryInstruction;
    RegistryVariable* registryVariable;
    RegistryFunctionGlobal& registryFunctionGlobal;
    RegistryFunctionLocal* registryFunctionLocal;
    ReturnContextCompilation* returnContextCompilation;
    RegistryArgument* registryArgument;
    RegistryClass* registryClass;
    llvm::BumpPtrAllocator* arena;
    std::string currentFilePath;
    std::string currentClassName;

public:
    ContextGenCode(
        RegistryType* p_registryType,
        LlvmBackend* p_backend,
        RegistryInstruction* p_registryInstruction,
        RegistryVariable* p_registryVariable,
        RegistryFunctionGlobal& p_registryFunctionGlobal,
        RegistryFunctionLocal* p_registryFunctionLocal,
        ReturnContextCompilation* p_returnContextCompilation,
        RegistryArgument* p_registryArgument,
        RegistryClass* p_registryClass,
        Symbol p_temporaryValue,
        llvm::BumpPtrAllocator* p_arena,
        std::string p_currentFilePath
    ) : registryType(p_registryType),
        temporaryValue(p_temporaryValue),
        backend(p_backend),
        registryInstruction(p_registryInstruction),
        registryVariable(p_registryVariable),
        registryFunctionGlobal(p_registryFunctionGlobal),
        registryFunctionLocal(p_registryFunctionLocal),
        returnContextCompilation(p_returnContextCompilation),
        registryArgument(p_registryArgument),
        registryClass(p_registryClass),
        arena(p_arena),
        currentFilePath(std::move(p_currentFilePath))
    {
        try {
            if (currentFilePath.empty()) {
                throw std::invalid_argument("The current file path cannot be empty");
            }
            if (p_registryType == nullptr)
            {
                throw std::invalid_argument("The file registry cannot be null");
            }
            if (p_backend == nullptr) {
                throw std::invalid_argument("The LLVM backend cannot be null");
            }
            if (p_registryInstruction == nullptr) {
                throw std::invalid_argument("The instruction registry cannot be null");
            }
            if (p_registryVariable == nullptr) {
                throw std::invalid_argument("The variable registry cannot be null");
            }
            if (p_registryFunctionLocal == nullptr) {
                throw std::invalid_argument("The local function registry cannot be null");
            }
            if (p_registryType == nullptr) {
                throw std::invalid_argument("The type registry cannot be null");
            }
            if (p_returnContextCompilation == nullptr) {
                throw std::invalid_argument("The return context compilation cannot be null");
            }
            if (p_registryArgument == nullptr) {
                throw std::invalid_argument("The argument registry cannot be null");
            }
            if (p_arena == nullptr) {
                throw std::invalid_argument("The arena cannot be null");
            }
            if (p_registryClass == nullptr) {
                throw std::invalid_argument("The class registry cannot be null");
            }
        } catch (const std::invalid_argument& e) {
            std::cerr << "Error during code generation context creation: " << e.what() << '\n';
            throw;
        }
    }

    
    void setTemporaryValue(Symbol p_temporaryValue) { temporaryValue = p_temporaryValue; }
    void setCurrentClassName(std::string p_currentClassName) { currentClassName = std::move(p_currentClassName); }

    // Getters
    [[nodiscard]] auto getRegistryType() const -> RegistryType* { return registryType; }
    [[nodiscard]] auto getTemporaryValue() const -> Symbol { return temporaryValue; }
    [[nodiscard]] auto getBackend() const -> LlvmBackend* { return backend; }
    [[nodiscard]] auto getRegistryInstruction() const -> RegistryInstruction* { return registryInstruction; }
    [[nodiscard]] auto getRegistryVariable() const -> RegistryVariable* { return registryVariable; }
    [[nodiscard]] auto getRegistryFunctionGlobal() const -> RegistryFunctionGlobal& { return registryFunctionGlobal; }
    [[nodiscard]] auto getRegistryFunctionLocal() const -> RegistryFunctionLocal* { return registryFunctionLocal; }
    [[nodiscard]] auto getReturnContextCompilation() const -> ReturnContextCompilation* { return returnContextCompilation; }
    [[nodiscard]] auto getRegistryArgument() const -> RegistryArgument* { return registryArgument; }
    [[nodiscard]] auto getRegistryClass() const -> RegistryClass* { return registryClass; }
    [[nodiscard]] auto getArena() const -> llvm::BumpPtrAllocator* { return arena; }
    [[nodiscard]] auto getCurrentFilePath() const -> const std::string& { return currentFilePath; }
    [[nodiscard]] auto getCurrentClassName() const -> const std::string& { return currentClassName; }
};

#endif /* C2537ED8_1CCF_4242_BDB0_B5ED5F2AD08F */
