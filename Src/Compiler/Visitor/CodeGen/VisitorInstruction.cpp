#include "Compiler/Visitor/CodeGen/VisitorGeneralGenCode.h"
#include "Compiler/AST/Nodes/NodeInstruction.h"

void GeneralVisitorGenCode::visiter(NodeInstruction* instruction)
{
    if (instruction != nullptr) {
        for (const auto& child : instruction->getChilds()) {
            if (child != nullptr) {
                child->accept(this);
            }
        }
    }
}
