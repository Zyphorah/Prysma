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

// Generation of the Declaration

class FunctionDeclarationGenerator {
private:
    ContextGenCode* _contextGenCode;
    NodeDeclarationFunction* _nodeDeclarationFunction;
    IVisitor* _visitorGeneralCodeGen;

    virtual auto createFunction() -> llvm::Function* = 0;
    virtual void handleConstructedArguments(llvm::Function* function, const ArgumentsCodeGen& args) = 0;

public:
    FunctionDeclarationGenerator(ContextGenCode* context, NodeDeclarationFunction* node, IVisitor* visitor);
    virtual ~FunctionDeclarationGenerator() = default;

    [[nodiscard]] auto getContextGenCode() const -> ContextGenCode* { return _contextGenCode; }
    [[nodiscard]] auto getNodeDeclarationFunction() const -> NodeDeclarationFunction* { return _nodeDeclarationFunction; }
    [[nodiscard]] auto getGeneralVisitorCodeGen() const -> IVisitor* { return _visitorGeneralCodeGen; }

    FunctionDeclarationGenerator(const FunctionDeclarationGenerator&) = delete;
    auto operator=(const FunctionDeclarationGenerator&) -> FunctionDeclarationGenerator& = delete;
    FunctionDeclarationGenerator(FunctionDeclarationGenerator&&) = delete;
    auto operator=(FunctionDeclarationGenerator&&) -> FunctionDeclarationGenerator& = delete;

    void declareFunction();
    
    static auto create(ContextGenCode* context, NodeDeclarationFunction* node, IVisitor* visitor) -> std::unique_ptr<FunctionDeclarationGenerator>;
};

class StandardFunctionDeclarationGenerator : public FunctionDeclarationGenerator {
private:
    auto createFunction() -> llvm::Function* override;
    void handleConstructedArguments(llvm::Function* function, const ArgumentsCodeGen& args) override;
public:
    using FunctionDeclarationGenerator::FunctionDeclarationGenerator;
};

class MethodFunctionDeclarationGenerator : public FunctionDeclarationGenerator {
private:
    auto createFunction() -> llvm::Function* override;
    void handleConstructedArguments(llvm::Function* function, const ArgumentsCodeGen& args) override;
public:
    using FunctionDeclarationGenerator::FunctionDeclarationGenerator;
};


// Generation of the Call
class FunctionCallGenerator {
private:
    ContextGenCode* _contextGenCode;
    IVisitor* _visitorGeneralCodeGen;

    virtual auto getLocalFunction(const std::string& functionName) -> const SymbolFunctionLocal* = 0;

public:
    FunctionCallGenerator(ContextGenCode* context, IVisitor* visitor);
    virtual ~FunctionCallGenerator() = default;

    [[nodiscard]] auto getContextGenCode() const -> ContextGenCode* { return _contextGenCode; }
    [[nodiscard]] auto getGeneralVisitorCodeGen() const -> IVisitor* { return _visitorGeneralCodeGen; }

    FunctionCallGenerator(const FunctionCallGenerator&) = delete;
    auto operator=(const FunctionCallGenerator&) -> FunctionCallGenerator& = delete;
    FunctionCallGenerator(FunctionCallGenerator&&) = delete;
    auto operator=(FunctionCallGenerator&&) -> FunctionCallGenerator& = delete;

    void generateCallFunction(NodeCallFunction* node);
    static auto create(ContextGenCode* context, IVisitor* visitor) -> std::unique_ptr<FunctionCallGenerator>;
};

class StandardFunctionCallGenerator : public FunctionCallGenerator {
private:
    auto getLocalFunction(const std::string& functionName) -> const SymbolFunctionLocal* override;
public:
    using FunctionCallGenerator::FunctionCallGenerator;
};

class MethodFunctionCallGenerator : public FunctionCallGenerator {
private:
    auto getLocalFunction(const std::string& functionName) -> const SymbolFunctionLocal* override;
public:
    using FunctionCallGenerator::FunctionCallGenerator;
};

// Management of Native (Built-in) functions

class RegistryBuiltIns {
public:
    static auto isBuiltIn(const std::string& name) -> bool;
    static void generateCall(const std::string& name, NodeCallFunction* node, ContextGenCode* context, IVisitor* visitor);
};

#endif /* D2944365_C1DD_41F8_A211_BFF33402A958 */
