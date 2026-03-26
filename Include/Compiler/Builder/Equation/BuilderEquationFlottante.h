#ifndef FLOATEQUATIONBUILDER_H
#define FLOATEQUATIONBUILDER_H

#include <llvm/Support/Allocator.h>
#include <memory>
#include <vector>

#include "Compiler/AST/Interfaces/IBuilderTree.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/Parser/Equation/ChainOfResponsibility.h"
#include "Compiler/Parser/Equation/ServiceParenthesis.h"
#include "Compiler/Parser/Equation/ManagerOperator.h"
#include "Compiler/AST/Registry/RegistrySymbole.h"
#include "Compiler/AST/Registry/RegistryExpression.h"

struct Token;

class BuilderFloatEquation : public IBuilderTree
{
private:
    RegistryExpression* _expressionRegistry;
    llvm::BumpPtrAllocator& _arena;
    std::unique_ptr<RegistrySymbol> _symbolRegistry;
    
    std::unique_ptr<OperatorManager> _additionManager;
    std::unique_ptr<OperatorManager> _subtractionManager;
    std::unique_ptr<OperatorManager> _multiplicationManager;
    std::unique_ptr<OperatorManager> _divisionManager;
    std::unique_ptr<OperatorManager> _moduloManager;
    std::unique_ptr<OperatorManager> _lessThanManager;
    std::unique_ptr<OperatorManager> _greaterThanManager;
    std::unique_ptr<OperatorManager> _lessThanOrEqualManager;
    std::unique_ptr<OperatorManager> _greaterThanOrEqualManager;
    std::unique_ptr<OperatorManager> _equalManager;
    std::unique_ptr<OperatorManager> _notEqualManager;
    std::unique_ptr<OperatorManager> _andManager;
    std::unique_ptr<OperatorManager> _orManager;

    std::unique_ptr<ParenthesisService> _parenthesisService;
    std::unique_ptr<ChainOfResponsibility> _chainOfResponsibility;
        
    IBuilderTree* _builderTree;

    void initializeRegistry();

public: 

    BuilderFloatEquation(RegistryExpression* expressionRegistry, llvm::BumpPtrAllocator& arena);
    
    ~BuilderFloatEquation() override;

    BuilderFloatEquation(const BuilderFloatEquation&) = delete;
    auto operator=(const BuilderFloatEquation&) -> BuilderFloatEquation& = delete;
    BuilderFloatEquation(BuilderFloatEquation&&) = delete;
    auto operator=(BuilderFloatEquation&&) -> BuilderFloatEquation& = delete;

    auto build(std::vector<Token>& tokens) -> INode* override;
    auto build(std::vector<Token>& tokens, int& index) -> INode* override;
    auto getArena() -> llvm::BumpPtrAllocator& override;
    
    [[nodiscard]] auto getBuilderTree() const -> IBuilderTree*;
};

#endif /* FLOATEQUATIONBUILDER_H */