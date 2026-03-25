#pragma once

#include "Compiler/AST/Interfaces/IBuilderTree.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/RegistrySymbole.h"
#include "Compiler/AST/Registry/RegistryExpression.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Parser/Equation/ChainOfResponsibility.h"
#include "Compiler/Parser/Equation/Interfaces/IManagerParenthesis.h"
#include <llvm/Support/Allocator.h>
#include <vector>


class BuilderTreeEquation : public IBuilderTree
{
private:
    ChainOfResponsibility* _chaineResponsabilite;
    RegistrySymbole* _registrySymbole;
    RegistryExpression* _registryExpression;
    IManagerParenthese* _managerParenthese;
    llvm::BumpPtrAllocator& _arena;
    Token _dernierToken;

public:
  
    BuilderTreeEquation(
        ChainOfResponsibility* chaineResponsabilite,
        RegistrySymbole* registrySymbole,
    RegistryExpression* registryExpression,
        IManagerParenthese* managerParenthese,
        llvm::BumpPtrAllocator& arena
    );
    
    auto construire(std::vector<Token> &equation) -> INode* override;
    auto construire(std::vector<Token>& tokens, int& index) -> INode* override;
    auto getArena() -> llvm::BumpPtrAllocator& override;
};
