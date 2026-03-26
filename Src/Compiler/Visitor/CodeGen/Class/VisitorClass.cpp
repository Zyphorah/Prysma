#include "Compiler/Visitor/CodeGen/VisitorGeneralGenCode.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/Utils/PrysmaCast.h"
#include <string>

void GeneralVisitorGenCode::visiter(NodeClass* nodeClass)
{
    std::string previousClassName = _contextGenCode->getCurrentClassName();
    _contextGenCode->setCurrentClassName(nodeClass->getNomClass().value);

    for (auto* member : nodeClass->getListMembers()) {
        if (prysma::isa<NodeDeclarationFunction>(member)) {
            member->accept(this);
        }
    }

    for (auto* builder : nodeClass->getBuilder()) {
        builder->accept(this);
    }

    _contextGenCode->setCurrentClassName(previousClassName);
}
