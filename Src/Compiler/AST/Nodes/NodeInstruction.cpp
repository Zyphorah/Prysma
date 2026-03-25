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

void NodeInstruction::ajouterInstruction(INode* child)
{
    if (child != nullptr) {
        childs.push_back(child);
    }
}

auto NodeInstruction::getTypeGenere() const -> NodeTypeGenere
{
    return NodeTypeGenere::CallFunction; 
}
