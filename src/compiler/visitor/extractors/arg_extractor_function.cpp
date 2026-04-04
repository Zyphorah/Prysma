//===-- arg_extractor_function.cpp -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#include "compiler/visitor/extractors/arg_extractor_function.h"
#include "compiler/ast/ast_genere.h"

void ArgExtractorFunction::visiter(NodeArgFunction* node) {
    arg = node;
}
