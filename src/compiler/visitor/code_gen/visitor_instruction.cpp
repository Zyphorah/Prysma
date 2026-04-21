#include "compiler/ast/ast_genere.h"
#include "compiler/ast/registry/node_component_registry.h"
#include "compiler/visitor/code_gen/visitor_general_gen_code.h"

void GeneralVisitorGenCode::visiter(NodeInstruction* instruction)
{
    if (instruction != nullptr) {
        auto& component = _contextGenCode->getNodeComponentRegistry()->get<NodeInstructionComponents>(instruction->getNodeId());

        for (const auto& child : component.getChildren()) {
            if (child != nullptr) {
                child->accept(this);
            }
        }
    }
}
