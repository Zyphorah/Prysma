#include "../../../../../build/generationCode/include/compiler/ast/ast_genere_copy.txt"
#include "compiler/ast/registry/node_component_registry.h"
#include "compiler/ast/registry/stack/registry_variable.h"
#include "compiler/lexer/lexer.h"
#include "compiler/visitor/code_gen/visitor_general_gen_code.h"
#include <llvm/IR/Value.h>

void GeneralVisitorGenCode::visiter(NodeRefVariable* nodeRefVariable) 
{
    auto& nodeData = _contextGenCode->getNodeComponentRegistry()->get<NodeRefVariableComponents>(nodeRefVariable->getNodeId());
    const auto& nodeToken = nodeData.getName();

    Symbol symbol = _contextGenCode->getRegistryVariable()->getVariable(nodeToken);
    llvm::Value* variable = symbol.getAddress();
    
    _contextGenCode->setTemporaryValue(Symbol(variable, _contextGenCode->getTemporaryValue().getType(), _contextGenCode->getTemporaryValue().getPointedElementType()));
    _contextGenCode->setTemporaryValue(Symbol(_contextGenCode->getTemporaryValue().getAddress(), symbol.getType(), _contextGenCode->getTemporaryValue().getPointedElementType()));
}
