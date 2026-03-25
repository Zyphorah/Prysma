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

BuilderEquationFlottante::BuilderEquationFlottante(RegistryExpression* registryExpression, llvm::BumpPtrAllocator& arena)
    : _registryExpression(registryExpression), _arena(arena)
{
    _registrySymbole = std::make_unique<RegistrySymbole>();

    _serviceParenthese = std::make_unique<ServiceParenthese>(_registrySymbole.get());

    _managerAddition = std::make_unique<ManagerOperateur>(TOKEN_PLUS);
    _managerSoustraction = std::make_unique<ManagerOperateur>(TOKEN_MOINS);
    _managerMultiplication = std::make_unique<ManagerOperateur>(TOKEN_ETOILE);
    _managerDivision = std::make_unique<ManagerOperateur>(TOKEN_SLASH);
    _managerModulo = std::make_unique<ManagerOperateur>(TOKEN_MODULO);
    _managerPlusPetit = std::make_unique<ManagerOperateur>(TOKEN_PLUS_PETIT);
    _managerPlusGrand = std::make_unique<ManagerOperateur>(TOKEN_PLUS_GRAND);
    _managerPlusPetitEgal = std::make_unique<ManagerOperateur>(TOKEN_PLUS_PETIT_EGAL);
    _managerPlusGrandEgal = std::make_unique<ManagerOperateur>(TOKEN_PLUS_GRAND_EGAL);
    _managerEgal = std::make_unique<ManagerOperateur>(TOKEN_EGAL_EGAL);
    _managerDifferent = std::make_unique<ManagerOperateur>(TOKEN_DIFFERENT);
    _managerEt = std::make_unique<ManagerOperateur>(TOKEN_ET);
    _managerOu = std::make_unique<ManagerOperateur>(TOKEN_OU);

    std::vector<ManagerOperateur*> operateurs = {
        _managerOu.get(),
        _managerEt.get(),
        _managerEgal.get(),
        _managerDifferent.get(),
        _managerPlusPetit.get(),
        _managerPlusGrand.get(),
        _managerPlusPetitEgal.get(),
        _managerPlusGrandEgal.get(),
        _managerAddition.get(), 
        _managerSoustraction.get(), 
        _managerMultiplication.get(), 
        _managerDivision.get(),
        _managerModulo.get()
    };
            
    _chaineResponsabilite = std::make_unique<ChainOfResponsibility>(_serviceParenthese.get(), operateurs);
                    
    _builderTree = std::unique_ptr<IBuilderTree>(
        new (_arena) BuilderTreeEquation(
            _chaineResponsabilite.get(), 
            _registrySymbole.get(), 
            _registryExpression,
            _serviceParenthese.get(),
            _arena
        )
    ).release();

    initialiserRegistry();
}

void BuilderEquationFlottante::initialiserRegistry()
{
    _registrySymbole->enregistryr(TOKEN_PLUS, [this](Token token) -> IExpression* { 
        return this->allouer<NodeOperation>(std::move(token)); 
    });

    _registrySymbole->enregistryr(TOKEN_MOINS, [this](Token token) -> IExpression* { 
        return this->allouer<NodeOperation>(std::move(token)); 
    });

    _registrySymbole->enregistryr(TOKEN_ETOILE, [this](Token token) -> IExpression* { 
        return this->allouer<NodeOperation>(std::move(token)); 
    });

    _registrySymbole->enregistryr(TOKEN_SLASH, [this](Token token) -> IExpression* { 
        return this->allouer<NodeOperation>(std::move(token)); 
    });
    _registrySymbole->enregistryr(TOKEN_PLUS_PETIT, [this](Token token) -> IExpression* { 
        return this->allouer<NodeOperation>(std::move(token)); 
    });
    _registrySymbole->enregistryr(TOKEN_PLUS_GRAND, [this](Token token) -> IExpression* { 
        return this->allouer<NodeOperation>(std::move(token)); 
    });
    _registrySymbole->enregistryr(TOKEN_PLUS_GRAND_EGAL, [this](Token token) -> IExpression* { 
        return this->allouer<NodeOperation>(std::move(token)); 
    });
    _registrySymbole->enregistryr(TOKEN_PLUS_PETIT_EGAL, [this](Token token) -> IExpression* { 
        return this->allouer<NodeOperation>(std::move(token));
    });
    _registrySymbole->enregistryr(TOKEN_MODULO, [this](Token token) -> IExpression* { 
        return this->allouer<NodeOperation>(std::move(token));
    });
    _registrySymbole->enregistryr(TOKEN_EGAL_EGAL, [this](Token token) -> IExpression* { 
        return this->allouer<NodeOperation>(std::move(token));
    });
    _registrySymbole->enregistryr(TOKEN_DIFFERENT, [this](Token token) -> IExpression* { 
        return this->allouer<NodeOperation>(std::move(token));
    });
    _registrySymbole->enregistryr(TOKEN_ET, [this](Token token) -> IExpression* { 
        return this->allouer<NodeOperation>(std::move(token));
    });
    _registrySymbole->enregistryr(TOKEN_OU, [this](Token token) -> IExpression* { 
        return this->allouer<NodeOperation>(std::move(token));
    });
}



auto BuilderEquationFlottante::construire(std::vector<Token> &tokens) -> INode*
{
    return _builderTree->construire(tokens);
}

auto BuilderEquationFlottante::recupererBuilderTree() const -> IBuilderTree*
{
    return _builderTree;
}

auto BuilderEquationFlottante::construire(std::vector<Token>& tokens, int& index) -> INode*
{
    return _builderTree->construire(tokens, index);
}

llvm::BumpPtrAllocator& BuilderEquationFlottante::getArena()
{
    return _arena;
}

BuilderEquationFlottante::~BuilderEquationFlottante()
{
    if (_builderTree != nullptr) {
        _builderTree->~IBuilderTree();
    }
}
