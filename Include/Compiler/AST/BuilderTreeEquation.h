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
    ChainOfResponsibility* _chainOfResponsibility;
    RegistrySymbol* _symbolRegistry;
    RegistryExpression* _expressionRegistry;
    IManagerParenthesis* _parenthesisManager;
    llvm::BumpPtrAllocator& _arena;
    Token _lastToken;

public:
  
    BuilderTreeEquation(
        ChainOfResponsibility* chainOfResponsibility,
        RegistrySymbol* symbolRegistry,
        RegistryExpression* expressionRegistry,
        IManagerParenthesis* parenthesisManager,
        llvm::BumpPtrAllocator& arena
    );
    
    auto build(std::vector<Token> &tokens) -> INode* override;
    auto build(std::vector<Token>& tokens, int& index) -> INode* override;
    auto getArena() -> llvm::BumpPtrAllocator& override;
};
