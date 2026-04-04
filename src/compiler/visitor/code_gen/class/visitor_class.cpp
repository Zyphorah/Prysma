//===-- visitor_class.cpp -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#include "compiler/visitor/code_gen/visitor_general_gen_code.h"
#include "compiler/ast/ast_genere.h"
#include "compiler/utils/prysma_cast.h"
#include <string>

void GeneralVisitorGenCode::visiter(NodeClass* nodeClass)
{
    std::string previousClassName = _contextGenCode->getCurrentClassName();
    _contextGenCode->setCurrentClassName(std::string(nodeClass->getNomClass().value));

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
