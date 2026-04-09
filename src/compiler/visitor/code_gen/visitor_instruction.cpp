#include "compiler/ast/registry/node_component_registry.h"
#include "compiler/visitor/code_gen/visitor_general_gen_code.h"
#include "compiler/ast/ast_genere.h"

void GeneralVisitorGenCode::visiter(NodeInstruction* instruction)
{
    if (instruction != nullptr) {
        auto& children = _contextGenCode->getNodeComponentRegistry()->get<AST_CHILD_COMPONENT>(instruction->getNodeId());

        for (const auto& child : children) {
            if (child != nullptr) {
                child->accept(this);
            }
        }
    }
}
