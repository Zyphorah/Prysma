#ifndef D8FAA486_F5BA_43FB_BFFC_AB9990B46458
#define D8FAA486_F5BA_43FB_BFFC_AB9990B46458

#include "Compiler/AST/Registry/ContextGenCode.h"
#include "Compiler/AST/Registry/ContextExpression.h"
#include "Compiler/AST/Registry/ContextParser.h"
#include "Compiler/AST/Registry/Stack/RegistryVariable.h"
#include "Compiler/AST/Registry/Stack/ReturnContextCompilation.h"
#include "Compiler/AST/Registry/RegistryArgument.h"
#include "Compiler/AST/Registry/RegistryExpression.h"
#include "Compiler/AST/Registry/RegistryClass.h"
#include "Compiler/AST/BuilderTreeInstruction.h"
#include "Compiler/AST/Registry/RegistryFunction.h"
#include "Compiler/AST/Registry/RegistryInstruction.h"
#include "Compiler/AST/Registry/RegistryType.h"
#include "Compiler/Builder/Equation/BuilderEquationFlottante.h"
#include "Compiler/Parser/ParserType.h"
#include "Compiler/LLVM/LlvmBackend.h"
#include "Compiler/Registry/RegistryFile.h"
#include <llvm/Support/Allocator.h>
#include <memory>
#include <string>

class BuilderFloatEquation;
class TypeParser;
class ConfigurationFacadeEnvironment
{
private:
    RegistryFunctionGlobal* _registryFunctionGlobal;

    llvm::BumpPtrAllocator _arena;

    std::unique_ptr<LlvmBackend> _backend;
    std::unique_ptr<RegistryInstruction> _registryInstruction;
    std::unique_ptr<RegistryVariable> _registryVariable;
    std::unique_ptr<RegistryFunctionLocal> _registryFunctionLocal;
    std::unique_ptr<RegistryType> _registryType;
    std::unique_ptr<ReturnContextCompilation> _returnContextCompilation;
    std::unique_ptr<RegistryArgument> _registryArgument;
    std::unique_ptr<RegistryClass> _registryClass;
    std::unique_ptr<ContextGenCode> _context;

    RegistryExpression* _registryExpression;
    BuilderTreeInstruction* _builderTreeInstruction;
    BuilderFloatEquation* _builderEquation;
    TypeParser* _parserType;
    ContextParser* _contextParser;
    ContextExpression* _contextExpression;

    void createRegistries();
    void createContext(const std::string& filePath);
    void createContextParser();
    void registerExternalFunctions();
    void registerBaseTypes();
    void registerExpressions();
    void registerInstructions();

public:
    explicit ConfigurationFacadeEnvironment(RegistryFunctionGlobal* registryFunctionGlobal, [[maybe_unused]] FileRegistry* registryFile);
    ~ConfigurationFacadeEnvironment();

    ConfigurationFacadeEnvironment(const ConfigurationFacadeEnvironment&) = delete;
    auto operator=(const ConfigurationFacadeEnvironment&) -> ConfigurationFacadeEnvironment& = delete;
    ConfigurationFacadeEnvironment(ConfigurationFacadeEnvironment&&) = delete;
    auto operator=(ConfigurationFacadeEnvironment&&) -> ConfigurationFacadeEnvironment& = delete;

    /// Initializes the entire compilation environment in a single step
    void initialize(const std::string& filePath);
    
    [[nodiscard]] auto getContext() const -> ContextGenCode*;
    auto getArena() -> llvm::BumpPtrAllocator&;
    [[nodiscard]] auto getBuilderTreeInstruction() const -> BuilderTreeInstruction*;
    [[nodiscard]] auto getBuilderEquation() const -> BuilderFloatEquation*;
};

#endif /* D8FAA486_F5BA_43FB_BFFC_AB9990B46458 */
