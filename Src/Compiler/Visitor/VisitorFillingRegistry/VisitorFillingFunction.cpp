#include "Compiler/Visitor/VisitorFillingRegistry/VisitorFillingRegistry.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Registry/RegistryClass.h"
#include "Compiler/AST/Registry/RegistryFunction.h"
#include "Compiler/AST/Registry/Types/IType.h"
#include <memory>
#include <string>
#include <utility>

void FillingVisitorRegistry::visiter(NodeDeclarationFunction* nodeDeclarationFunction)
{
    IType* returnType = nodeDeclarationFunction->getTypeReturn();
    std::string functionName = nodeDeclarationFunction->getNom();
    
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