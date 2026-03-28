#include "compiler/visitor/code_gen/visitor_general_gen_code.h"
#include "compiler/ast/ast_genere.h"

void GeneralVisitorGenCode::visiter(NodeInstruction* instruction)
{
    if (instruction != nullptr) {
        for (const auto& child : instruction->getChildren()) {
            if (child != nullptr) {
                child->accept(this);
            }
        }
    }
}
