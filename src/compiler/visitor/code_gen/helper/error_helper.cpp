//===-- error_helper.cpp -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#include "compiler/visitor/code_gen/helper/error_helper.h"
#include <stdexcept>
#include <string>

void ErrorHelper::compilationError(const std::string& message) {
    throw std::runtime_error("Semantic error: " + message);
}
