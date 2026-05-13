//===-- visitor_filling_function.cpp ----------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#include "compiler/visitor/visitor_filling_registry/visitor_filling_registry.h"
#include "compiler/ast/ast_genere.h"
#include "compiler/ast/registry/registry_class.h"
#include "compiler/ast/registry/registry_function.h"
#include "compiler/ast/registry/types/i_type.h"
#include <llvm/ADT/StringRef.h>
#include <memory>
#include <string>
#include <utility>

void FillingVisitorRegistry::visiter(NodeDeclarationFunction* nodeDeclarationFunction)
{
    IType* returnType = nodeDeclarationFunction->getTypeReturn();
    llvm::StringRef functionName = nodeDeclarationFunction->getNom().value;
    
    if (_contextGenCode->getCurrentClassName() != "") {
        // class context (method)
        auto functionSymbol = std::make_unique<SymbolFunctionLocal>();
        functionSymbol->setReturnType(returnType);
        functionSymbol->setNode(nodeDeclarationFunction);

        std::string className = _contextGenCode->getCurrentClassName();
        auto* classInfo = _contextGenCode->getRegistryClassGlobal()->get(className).get();
        classInfo->getMaterializedFunctionRegistry()->registerElement(functionName, std::move(functionSymbol));
    } else {
        // global context (global function)
        auto functionSymbol = std::make_unique<SymbolFunctionGlobal>(returnType,nodeDeclarationFunction);
        _contextGenCode->getRegistryFunctionGlobal()->registerElement(functionName, std::move(functionSymbol));
    }
}
