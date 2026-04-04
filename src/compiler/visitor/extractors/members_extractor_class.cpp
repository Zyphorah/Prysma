//===-- members_extractor_class.cpp -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#include "compiler/visitor/extractors/members_extractor_class.h"
#include "compiler/ast/ast_genere.h"

void MembersExtractorClass::visiter(NodeDeclarationFunction* node) { 
    methods.push_back(node); 
}

void MembersExtractorClass::visiter(NodeDeclarationVariable* node) { 
    variables.push_back(node); 
}

void MembersExtractorClass::visiter(NodeClass* node) { 
    className = node->getNomClass().value; 
}
