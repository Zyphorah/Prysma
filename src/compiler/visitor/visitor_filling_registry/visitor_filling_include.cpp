//===-- visitor_filling_include.cpp -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#include "compiler/visitor/visitor_filling_registry/visitor_filling_registry.h"
#include "compiler/ast/utils/orchestrator_include/orchestrator_include.h" 
#include "compiler/ast/ast_genere.h" 
#include <filesystem>
#include <string>

void FillingVisitorRegistry::visiter(NodeInclude* nodeInclude)
{
    std::filesystem::path parentFilePath(_contextGenCode->getCurrentFilePath());
    std::filesystem::path parentDirectory = parentFilePath.parent_path();
    std::filesystem::path absolutePath = parentDirectory / std::string(nodeInclude->getPath().value);
    _orchestrator->includeFile(absolutePath.string());
}

