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

class BuilderEquationFlottante : public IBuilderTree
{
private:
    std::unique_ptr<RegistrySymbole> _registrySymbole;
    
    std::unique_ptr<ManagerOperateur> _managerAddition;
    std::unique_ptr<ManagerOperateur> _managerSoustraction;
    std::unique_ptr<ManagerOperateur> _managerMultiplication;
    std::unique_ptr<ManagerOperateur> _managerDivision;
    std::unique_ptr<ManagerOperateur> _managerModulo;
    std::unique_ptr<ManagerOperateur> _managerPlusPetit;
    std::unique_ptr<ManagerOperateur> _managerPlusGrand;
    std::unique_ptr<ManagerOperateur> _managerPlusPetitEgal;
    std::unique_ptr<ManagerOperateur> _managerPlusGrandEgal;
    std::unique_ptr<ManagerOperateur> _managerEgal;
    std::unique_ptr<ManagerOperateur> _managerDifferent;
    std::unique_ptr<ManagerOperateur> _managerEt;
    std::unique_ptr<ManagerOperateur> _managerOu;

    std::unique_ptr<ServiceParenthese> _serviceParenthese;
    std::unique_ptr<ChainOfResponsibility> _chaineResponsabilite;
        
    IBuilderTree* _builderTree;
    RegistryExpression* _registryExpression;
    llvm::BumpPtrAllocator& _arena;


    void initialiserRegistry();

public: 

    BuilderEquationFlottante(RegistryExpression* registryExpression, llvm::BumpPtrAllocator& arena);
    
    ~BuilderEquationFlottante() override;

    BuilderEquationFlottante(const BuilderEquationFlottante&) = delete;
    auto operator=(const BuilderEquationFlottante&) -> BuilderEquationFlottante& = delete;
    BuilderEquationFlottante(BuilderEquationFlottante&&) = delete;
    auto operator=(BuilderEquationFlottante&&) -> BuilderEquationFlottante& = delete;

    auto construire(std::vector<Token>& tokens) -> INode* override;
    auto construire(std::vector<Token>& tokens, int& index) -> INode* override;
    auto getArena() -> llvm::BumpPtrAllocator& override;
    
    [[nodiscard]] auto recupererBuilderTree() const -> IBuilderTree*;
};

#endif /* FLOATEQUATIONBUILDER_H */