#ifndef F2141F07_2C85_4ADB_9BC9_A909EBD34394
#define F2141F07_2C85_4ADB_9BC9_A909EBD34394

#include "RegistryGeneric.h"
#include <llvm-18/llvm/IR/Function.h>
#include <mutex>
#include <string>
#include <memory>

class IType;
class NodeDeclarationFunction;

class ISymboleRegistryFunction
{
    public:
        enum class SymboleType { Globale, Locale };
        ISymboleRegistryFunction() = default;
        ISymboleRegistryFunction(const ISymboleRegistryFunction&) = delete;
        auto operator=(const ISymboleRegistryFunction&) -> ISymboleRegistryFunction& = delete;
        ISymboleRegistryFunction(ISymboleRegistryFunction&&) = delete;
        auto operator=(ISymboleRegistryFunction&&) -> ISymboleRegistryFunction& = delete;
        virtual ~ISymboleRegistryFunction() = default;
        [[nodiscard]] virtual SymboleType getType() const = 0;
};

// Fait office de struct pour stocker les functions globales donc 
// Je met en public 
// Pas de llvm::Function* ici, thread safe pour le registry global
class SymboleFunctionGlobale : public ISymboleRegistryFunction {
public: 
    IType* typeReturn = nullptr;
    NodeDeclarationFunction* node = nullptr;
    
    [[nodiscard]] auto getType() const -> SymboleType override { return SymboleType::Globale; }
    [[nodiscard]] static auto classof(const ISymboleRegistryFunction* s) -> bool { 
        return s->getType() == SymboleType::Globale; 
    }
};

// Registry local avec llvm::Function* pour la génération de code dans un thread
class SymboleFunctionLocale : public ISymboleRegistryFunction {
public:
    llvm::Function* function = nullptr;
    IType* typeReturn = nullptr;
    NodeDeclarationFunction* node = nullptr;

    [[nodiscard]] auto getType() const -> SymboleType override { return SymboleType::Locale; }
    [[nodiscard]] static auto classof(const ISymboleRegistryFunction* s) -> bool { 
        return s->getType() == SymboleType::Locale; 
    }
};

class RegistryFunctionGlobale : public RegistryGeneric<std::string, std::unique_ptr<ISymboleRegistryFunction>, std::mutex>
{
public:
    RegistryFunctionGlobale() = default;
    RegistryFunctionGlobale(const RegistryFunctionGlobale&) = delete;
    auto operator=(const RegistryFunctionGlobale&) -> RegistryFunctionGlobale& = delete;
    RegistryFunctionGlobale(RegistryFunctionGlobale&&) = delete;
    auto operator=(RegistryFunctionGlobale&&) -> RegistryFunctionGlobale& = delete;
    ~RegistryFunctionGlobale() override = default;
};

class RegistryFunctionLocale : public RegistryGeneric<std::string, std::unique_ptr<ISymboleRegistryFunction>>
{
public:
    RegistryFunctionLocale() = default;
    RegistryFunctionLocale(const RegistryFunctionLocale&) = delete;
    auto operator=(const RegistryFunctionLocale&) -> RegistryFunctionLocale& = delete;
    RegistryFunctionLocale(RegistryFunctionLocale&&) = delete;
    auto operator=(RegistryFunctionLocale&&) -> RegistryFunctionLocale& = delete;
    ~RegistryFunctionLocale() override = default;
};

#endif /* F2141F07_2C85_4ADB_9BC9_A909EBD34394 */
