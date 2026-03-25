#ifndef D2944365_C1DD_41F8_A211_BFF33402A958
#define D2944365_C1DD_41F8_A211_BFF33402A958

#include "Compiler/AST/Registry/ContextGenCode.h"
#include "Compiler/AST/Registry/RegistryFunction.h"
#include "Compiler/Visitor/Interfaces/IVisitor.h"
#include <llvm-18/llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <memory>
#include <vector>
#include <string>

class GeneralVisitorGenCode;
class NodeArgFunction;
class NodeDeclarationFunction;
class NodeCallFunction;

struct ArgumentsCodeGen {
    std::vector<llvm::Type*> argTypes;
    std::vector<NodeArgFunction*> arguments;
};

// Génération de la Déclaration

class GenerateurDeclarationFunction {
private:
    ContextGenCode* _contextGenCode;
    NodeDeclarationFunction* _nodeDeclarationFunction;
    IVisitor* _visitorGeneralCodeGen;

    virtual auto creerFunction() -> llvm::Function* = 0;
    virtual void traiterArgumentsConstruit(llvm::Function* function, const ArgumentsCodeGen& args) = 0;

public:
    GenerateurDeclarationFunction(ContextGenCode* context, NodeDeclarationFunction* node, IVisitor* visitor);
    virtual ~GenerateurDeclarationFunction() = default;

    [[nodiscard]] auto getContextGenCode() const -> ContextGenCode* { return _contextGenCode; }
    [[nodiscard]] auto getNodeDeclarationFunction() const -> NodeDeclarationFunction* { return _nodeDeclarationFunction; }
    [[nodiscard]] auto getGeneralVisitorCodeGen() const -> IVisitor* { return _visitorGeneralCodeGen; }

    GenerateurDeclarationFunction(const GenerateurDeclarationFunction&) = delete;
    auto operator=(const GenerateurDeclarationFunction&) -> GenerateurDeclarationFunction& = delete;
    GenerateurDeclarationFunction(GenerateurDeclarationFunction&&) = delete;
    auto operator=(GenerateurDeclarationFunction&&) -> GenerateurDeclarationFunction& = delete;

    void declarerFunction();
    
    static auto creer(ContextGenCode* context, NodeDeclarationFunction* node, IVisitor* visitor) -> std::unique_ptr<GenerateurDeclarationFunction>;
};

class GenerateurDeclarationStandard : public GenerateurDeclarationFunction {
private:
    auto creerFunction() -> llvm::Function* override;
    void traiterArgumentsConstruit(llvm::Function* function, const ArgumentsCodeGen& args) override;
public:
    using GenerateurDeclarationFunction::GenerateurDeclarationFunction;
};

class GenerateurDeclarationMethode : public GenerateurDeclarationFunction {
private:
    auto creerFunction() -> llvm::Function* override;
    void traiterArgumentsConstruit(llvm::Function* function, const ArgumentsCodeGen& args) override;
public:
    using GenerateurDeclarationFunction::GenerateurDeclarationFunction;
};


// Génération de l'Call
class GenerateurCallFunction {
private:
    ContextGenCode* _contextGenCode;
    IVisitor* _visitorGeneralCodeGen;

    virtual auto obtenirFunctionLocale(const std::string& nomFunction) -> const SymboleFunctionLocale* = 0;

public:
    GenerateurCallFunction(ContextGenCode* context, IVisitor* visitor);
    virtual ~GenerateurCallFunction() = default;

    [[nodiscard]] auto getContextGenCode() const -> ContextGenCode* { return _contextGenCode; }
    [[nodiscard]] auto getGeneralVisitorCodeGen() const -> IVisitor* { return _visitorGeneralCodeGen; }

    GenerateurCallFunction(const GenerateurCallFunction&) = delete;
    auto operator=(const GenerateurCallFunction&) -> GenerateurCallFunction& = delete;
    GenerateurCallFunction(GenerateurCallFunction&&) = delete;
    auto operator=(GenerateurCallFunction&&) -> GenerateurCallFunction& = delete;

    void generatedrCallFunction(NodeCallFunction* node);
    static auto creer(ContextGenCode* context, IVisitor* visitor) -> std::unique_ptr<GenerateurCallFunction>;
};

class GenerateurCallStandard : public GenerateurCallFunction {
private:
    const SymboleFunctionLocale* obtenirFunctionLocale(const std::string& nomFunction) override;
public:
    using GenerateurCallFunction::GenerateurCallFunction;
};

class GenerateurCallMethode : public GenerateurCallFunction {
private:
    const SymboleFunctionLocale* obtenirFunctionLocale(const std::string& nomFunction) override;
public:
    using GenerateurCallFunction::GenerateurCallFunction;
};

// Gestion des functions Natives (Built-ins)

class RegistryBuiltIns {
public:
    static auto estBuiltIn(const std::string& nom) -> bool;
    static void generatedrCall(const std::string& nom, NodeCallFunction* node, ContextGenCode* context, IVisitor* visitor);
};

#endif /* D2944365_C1DD_41F8_A211_BFF33402A958 */
