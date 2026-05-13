//===-- registry_function.h -------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#ifndef F2141F07_2C85_4ADB_9BC9_A909EBD34394
#define F2141F07_2C85_4ADB_9BC9_A909EBD34394

#include "registry_generic.h"
#include <cstdint>
#include <llvm-18/llvm/ADT/StringRef.h>
#include <llvm-18/llvm/IR/Function.h>
#include <mutex>
#include <memory>

class IType;
class NodeDeclarationFunction;

class IFunctionSymbolRegistry
{
    public:
        enum class SymbolType : std::uint8_t { Global, Local };
        IFunctionSymbolRegistry() = default;
        IFunctionSymbolRegistry(const IFunctionSymbolRegistry&) = delete;
        auto operator=(const IFunctionSymbolRegistry&) -> IFunctionSymbolRegistry& = delete;
        IFunctionSymbolRegistry(IFunctionSymbolRegistry&&) = delete;
        auto operator=(IFunctionSymbolRegistry&&) -> IFunctionSymbolRegistry& = delete;
        virtual ~IFunctionSymbolRegistry() = default;
        [[nodiscard]] virtual auto getType() const -> SymbolType = 0;
};

// Acts as a struct to store global functions
// Public members
// No llvm::Function* here, thread safe for the global registry
class SymbolFunctionGlobal : public IFunctionSymbolRegistry {
private:
    IType* _returnType;
    NodeDeclarationFunction* _node;
public:  
    explicit SymbolFunctionGlobal(
        IType* returnType,
        NodeDeclarationFunction* node
    ):
    _returnType(returnType),
    _node(node)
    {}; 
    [[nodiscard]] auto getReturnType() const -> IType*{ return _returnType;}
    [[nodiscard]] auto getNode() const -> NodeDeclarationFunction*{return _node;}
    
    [[nodiscard]] auto getType() const -> SymbolType override { return SymbolType::Global; }
    [[nodiscard]] static auto classof(const IFunctionSymbolRegistry* symbole) -> bool { 
        return symbole->getType() == SymbolType::Global; 
    }
};

// Local registry with llvm::Function* for code generation in a thread
class SymbolFunctionLocal : public IFunctionSymbolRegistry {
private:
    llvm::Function* _function;
    IType* _returnType;
    NodeDeclarationFunction* _node;
public:

    SymbolFunctionLocal() : _function(nullptr), _returnType(nullptr), _node(nullptr) {}

    explicit SymbolFunctionLocal(
        llvm::Function* function,
        IType* returnType,
        NodeDeclarationFunction* node
    ) : 
        _function(function),
        _returnType(returnType),
        _node(node)
    {};
    
    [[nodiscard]] auto getFunction() const -> llvm::Function* {return _function;}
    [[nodiscard]] auto getReturnType() const -> IType* {return _returnType;}
    [[nodiscard]] auto getNode() const -> NodeDeclarationFunction* {return _node;}

    // TODO : bon à regarder je ne sais pas si c'est la meilleur solution ou peut être créer un nouveau objet serais préférable ? 
    auto setFunction(llvm::Function* function) -> void { _function = function; }
    auto setReturnType(IType* returnType) -> void { _returnType = returnType; }
    auto setNode(NodeDeclarationFunction* node) -> void { _node = node; }
 
    [[nodiscard]] auto getType() const -> SymbolType override { return SymbolType::Local; }
    [[nodiscard]] static auto classof(const IFunctionSymbolRegistry* symbole) -> bool { 
        return symbole->getType() == SymbolType::Local; 
    }
};

class RegistryFunctionGlobal : public RegistryGeneric<llvm::StringRef, std::unique_ptr<IFunctionSymbolRegistry>, std::mutex>
{
public:
    RegistryFunctionGlobal() = default;
    RegistryFunctionGlobal(const RegistryFunctionGlobal&) = delete;
    auto operator=(const RegistryFunctionGlobal&) -> RegistryFunctionGlobal& = delete;
    RegistryFunctionGlobal(RegistryFunctionGlobal&&) = delete;
    auto operator=(RegistryFunctionGlobal&&) -> RegistryFunctionGlobal& = delete;
    ~RegistryFunctionGlobal() override = default;
};

class MaterializedFunctionRegistry : public RegistryGeneric<llvm::StringRef, std::unique_ptr<IFunctionSymbolRegistry>>
{
public:
    MaterializedFunctionRegistry() = default;
    MaterializedFunctionRegistry(const MaterializedFunctionRegistry&) = delete;
    auto operator=(const MaterializedFunctionRegistry&) -> MaterializedFunctionRegistry& = delete;
    MaterializedFunctionRegistry(MaterializedFunctionRegistry&&) = delete;
    auto operator=(MaterializedFunctionRegistry&&) -> MaterializedFunctionRegistry& = delete;
    ~MaterializedFunctionRegistry() override = default;
};

#endif /* F2141F07_2C85_4ADB_9BC9_A909EBD34394 */
