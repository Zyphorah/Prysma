#include "compiler/visitor/visitor_filling_registry/visitor_filling_registry.h"
#include "compiler/ast/ast_genere.h"
#include "compiler/ast/registry/registry_class.h"
#include "compiler/ast/registry/registry_function.h"
#include "compiler/ast/registry/types/i_type.h"
#include <memory>
#include <string>
#include <utility>

void FillingVisitorRegistry::visiter(NodeDeclarationFunction* nodeDeclarationFunction)
{
    IType* returnType = nodeDeclarationFunction->getTypeReturn();
    std::string functionName = nodeDeclarationFunction->getNom().value.str();
    
    if (_contextGenCode->getCurrentClassName() != "") {
        // class context (method)
        auto functionSymbol = std::make_unique<SymbolFunctionLocal>();
        functionSymbol->returnType = returnType;
        functionSymbol->node = nodeDeclarationFunction;

        std::string className = _contextGenCode->getCurrentClassName();
        auto* classInfo = _contextGenCode->getRegistryClass()->get(className).get();
        classInfo->getRegistryFunctionLocal()->registerElement(functionName, std::move(functionSymbol));
    } else {
        // global context (global function)
        auto functionSymbol = std::make_unique<SymbolFunctionGlobal>();
        functionSymbol->returnType = returnType;
        functionSymbol->node = nodeDeclarationFunction;

        _contextGenCode->getRegistryFunctionGlobal()->registerElement(functionName, std::move(functionSymbol));
    }
}