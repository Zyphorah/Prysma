#include "compiler/ast/ast_genere.h"
#include "compiler/ast/registry/node_component_registry.h"
#include "compiler/visitor/code_gen/visitor_general_gen_code.h"
#include "compiler/llvm/gestion_variable.h"

void GeneralVisitorGenCode::visiter(NodeUnRefVariable* nodeUnRefVariable)
{
    VariableLoader loader(_contextGenCode);

    auto& nodeData = _contextGenCode->getNodeComponentRegistry()->get<NodeUnRefVariableComponents>(nodeUnRefVariable->getNodeId());
    _contextGenCode->setTemporaryValue(loader.loadUnref(nodeData.getName().value));
}