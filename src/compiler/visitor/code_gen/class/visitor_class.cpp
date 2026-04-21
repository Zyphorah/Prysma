#include "compiler/ast/ast_genere.h"
#include "compiler/ast/registry/node_component_registry.h"
#include "compiler/visitor/code_gen/visitor_general_gen_code.h"
#include "compiler/utils/prysma_cast.h"
#include "compiler/visitor/interfaces/i_visitor.h"
#include <string>

void GeneralVisitorGenCode::visiter(NodeClass* nodeClass)
{
    std::string previousClassName = _contextGenCode->getCurrentClassName();

    auto& nodeData = _contextGenCode->getNodeComponentRegistry()->get<NodeClassComponents>(
        nodeClass->getNodeId()
    );

    _contextGenCode->setCurrentClassName(std::string(nodeData.getName().value));

    for (auto* member : nodeData.getMembers()) {
        if (prysma::isa<NodeDeclarationFunction>(member)) {
            member->accept(this);
        }
    }

    for (auto* builder : nodeData.getBuilder()) {
        builder->accept(this);
    }

    _contextGenCode->setCurrentClassName(previousClassName);
}
