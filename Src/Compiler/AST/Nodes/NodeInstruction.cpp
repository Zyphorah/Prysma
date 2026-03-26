#include "Compiler/AST/Nodes/NodeInstruction.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/Visitor/Interfaces/IVisitor.h"
#include "Compiler/AST/AST_Genere.h"

NodeInstruction::NodeInstruction()
= default;

void NodeInstruction::accept(IVisitor* visitor)
{
    visitor->visiter(this);
}

void NodeInstruction::addInstruction(INode* child)
{
    if (child != nullptr) {
        children.push_back(child);
    }
}

auto NodeInstruction::getGeneratedType() const -> NodeTypeGenerated
{
    return NodeTypeGenerated::CallFunction; 
}
