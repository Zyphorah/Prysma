#include "Compiler/AST/BuilderTreeEquation.h"
#include "Compiler/Builder/Equation/BuilderEquationFlottante.h"
#include "Compiler/AST/Interfaces/IBuilderTree.h"
#include "Compiler/AST/Nodes/Interfaces/IExpression.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/RegistryExpression.h"
#include "Compiler/Parser/Equation/ChainOfResponsibility.h"
#include "Compiler/Parser/Equation/ManagerOperator.h"
#include "Compiler/Parser/Equation/ServiceParenthesis.h"
#include "Compiler/Lexer/TokenType.h"
#include <llvm/Support/Allocator.h>
#include <memory>
#include <utility>
#include <vector>

// Floating-point equation builder
BuilderFloatEquation::BuilderFloatEquation(RegistryExpression* registryExpression, llvm::BumpPtrAllocator& arena)
    : _expressionRegistry(registryExpression), _arena(arena)
{
    _symbolRegistry = std::make_unique<RegistrySymbol>();

    _parenthesisService = std::make_unique<ParenthesisService>(_symbolRegistry.get());

    _additionManager = std::make_unique<OperatorManager>(TOKEN_PLUS);
    _subtractionManager = std::make_unique<OperatorManager>(TOKEN_MINUS);
    _multiplicationManager = std::make_unique<OperatorManager>(TOKEN_STAR);
    _divisionManager = std::make_unique<OperatorManager>(TOKEN_SLASH);
    _moduloManager = std::make_unique<OperatorManager>(TOKEN_MODULO);
    _lessThanManager = std::make_unique<OperatorManager>(TOKEN_LESS);
    _greaterThanManager = std::make_unique<OperatorManager>(TOKEN_GREATER);
    _lessThanOrEqualManager = std::make_unique<OperatorManager>(TOKEN_LESS_EQUAL);
    _greaterThanOrEqualManager = std::make_unique<OperatorManager>(TOKEN_GREATER_EQUAL);
    _equalManager = std::make_unique<OperatorManager>(TOKEN_EQUAL_EQUAL);
    _notEqualManager = std::make_unique<OperatorManager>(TOKEN_NOT_EQUAL);
    _andManager = std::make_unique<OperatorManager>(TOKEN_AND);
    _orManager = std::make_unique<OperatorManager>(TOKEN_OR);

    std::vector<OperatorManager*> operators = {
        _orManager.get(),
        _andManager.get(),
        _equalManager.get(),
        _notEqualManager.get(),
        _lessThanManager.get(),
        _greaterThanManager.get(),
        _lessThanOrEqualManager.get(),
        _greaterThanOrEqualManager.get(),
        _additionManager.get(), 
        _subtractionManager.get(), 
        _multiplicationManager.get(), 
        _divisionManager.get(),
        _moduloManager.get()
    };
            
    _chainOfResponsibility = std::make_unique<ChainOfResponsibility>(_parenthesisService.get(), operators);
                    
    _builderTree = std::unique_ptr<IBuilderTree>(
        new (_arena) BuilderTreeEquation(
            _chainOfResponsibility.get(), 
            _symbolRegistry.get(), 
            _expressionRegistry,
            _parenthesisService.get(),
            _arena
        )
    ).release();

    initializeRegistry();
}

void BuilderFloatEquation::initializeRegistry()
{
    _symbolRegistry->registerSymbol(TOKEN_PLUS, [this](Token token) -> IExpression* { 
        return this->allocate<NodeOperation>(std::move(token)); 
    });

    _symbolRegistry->registerSymbol(TOKEN_MINUS, [this](Token token) -> IExpression* { 
        return this->allocate<NodeOperation>(std::move(token)); 
    });

    _symbolRegistry->registerSymbol(TOKEN_STAR, [this](Token token) -> IExpression* { 
        return this->allocate<NodeOperation>(std::move(token)); 
    });

    _symbolRegistry->registerSymbol(TOKEN_SLASH, [this](Token token) -> IExpression* { 
        return this->allocate<NodeOperation>(std::move(token)); 
    });
    _symbolRegistry->registerSymbol(TOKEN_LESS, [this](Token token) -> IExpression* { 
        return this->allocate<NodeOperation>(std::move(token)); 
    });
    _symbolRegistry->registerSymbol(TOKEN_GREATER, [this](Token token) -> IExpression* { 
        return this->allocate<NodeOperation>(std::move(token)); 
    });
    _symbolRegistry->registerSymbol(TOKEN_GREATER_EQUAL, [this](Token token) -> IExpression* { 
        return this->allocate<NodeOperation>(std::move(token)); 
    });
    _symbolRegistry->registerSymbol(TOKEN_LESS_EQUAL, [this](Token token) -> IExpression* { 
        return this->allocate<NodeOperation>(std::move(token));
    });
    _symbolRegistry->registerSymbol(TOKEN_MODULO, [this](Token token) -> IExpression* { 
        return this->allocate<NodeOperation>(std::move(token));
    });
    _symbolRegistry->registerSymbol(TOKEN_EQUAL_EQUAL, [this](Token token) -> IExpression* { 
        return this->allocate<NodeOperation>(std::move(token));
    });
    _symbolRegistry->registerSymbol(TOKEN_NOT_EQUAL, [this](Token token) -> IExpression* { 
        return this->allocate<NodeOperation>(std::move(token));
    });
    _symbolRegistry->registerSymbol(TOKEN_AND, [this](Token token) -> IExpression* { 
        return this->allocate<NodeOperation>(std::move(token));
    });
    _symbolRegistry->registerSymbol(TOKEN_OR, [this](Token token) -> IExpression* { 
        return this->allocate<NodeOperation>(std::move(token));
    });
}

auto BuilderFloatEquation::build(std::vector<Token> &tokens) -> INode*
{
    return _builderTree->build(tokens);
}

auto BuilderFloatEquation::getBuilderTree() const -> IBuilderTree*
{
    return _builderTree;
}

auto BuilderFloatEquation::build(std::vector<Token>& tokens, int& index) -> INode*
{
    return _builderTree->build(tokens, index);
}

llvm::BumpPtrAllocator& BuilderFloatEquation::getArena()
{
    return _arena;
}

BuilderFloatEquation::~BuilderFloatEquation()
{
    if (_builderTree != nullptr) {
        _builderTree->~IBuilderTree();
    }
}
