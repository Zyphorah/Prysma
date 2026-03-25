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

class ConfigurationFacadeEnvironnement
{
private:
    RegistryFunctionGlobale* _registryFunctionGlobale;
    RegistryFile* _registryFile;

    llvm::BumpPtrAllocator _arena;

    std::unique_ptr<LlvmBackend> _backend;
    std::unique_ptr<RegistryInstruction> _registryInstruction;
    std::unique_ptr<RegistryVariable> _registryVariable;
    std::unique_ptr<RegistryFunctionLocale> _registryFunctionLocale;
    std::unique_ptr<RegistryType> _registryType;
    std::unique_ptr<ReturnContextCompilation> _returnContextCompilation;
    std::unique_ptr<RegistryArgument> _registryArgument;
    std::unique_ptr<RegistryClass> _registryClass;
    std::unique_ptr<ContextGenCode> _context;

    RegistryExpression* _registryExpression;
    BuilderTreeInstruction* _builderTreeInstruction;
    BuilderEquationFlottante* _builderEquation;
    ParserType* _parserType;
    ContextParser* _contextParser;
    ContextExpression* _contexteExpression;

    void creerRegistrys();
    void creerContext(const std::string& cheminFile);
    void creerContextParser();
    void enregistryrFunctionsExternes();
    void enregistryrTypesDeBase();
    void enregistryrExpressions();
    void enregistryrInstructions();

public:
    explicit ConfigurationFacadeEnvironnement(RegistryFunctionGlobale* registryFunctionGlobale, RegistryFile* registryFile);
    ~ConfigurationFacadeEnvironnement();

    ConfigurationFacadeEnvironnement(const ConfigurationFacadeEnvironnement&) = delete;
    auto operator=(const ConfigurationFacadeEnvironnement&) -> ConfigurationFacadeEnvironnement& = delete;
    ConfigurationFacadeEnvironnement(ConfigurationFacadeEnvironnement&&) = delete;
    auto operator=(ConfigurationFacadeEnvironnement&&) -> ConfigurationFacadeEnvironnement& = delete;

    /// Initialise tout l'environnement de compilation en une seule étape
    void initialiser(const std::string& cheminFile);
    
    [[nodiscard]] auto getContext() const -> ContextGenCode*;
    auto getArena() -> llvm::BumpPtrAllocator&;
    [[nodiscard]] auto getBuilderTreeInstruction() const -> BuilderTreeInstruction*;
    [[nodiscard]] auto getBuilderEquation() const -> BuilderEquationFlottante*;
};

#endif /* D8FAA486_F5BA_43FB_BFFC_AB9990B46458 */
