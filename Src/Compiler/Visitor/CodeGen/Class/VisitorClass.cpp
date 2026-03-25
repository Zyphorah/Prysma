#include "Compiler/Visitor/CodeGen/VisitorGeneralGenCode.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/Utils/PrysmaCast.h"
#include <string>

void GeneralVisitorGenCode::visiter(NodeClass* nodeClass)
{
    std::string ancienneClasse = _contextGenCode->getNomClasseCourante();
    _contextGenCode->modifierNomClasseCourante( nodeClass->getNomClass().value);

    for (auto* membre : nodeClass->getListMembers()) {
        if (prysma::isa<NodeDeclarationFunction>(membre)) {
            membre->accept(this);
        }
    }

    for (auto* builder : nodeClass->getBuilder()) {
        builder->accept(this);
    }

    _contextGenCode->modifierNomClasseCourante( ancienneClasse);
}
