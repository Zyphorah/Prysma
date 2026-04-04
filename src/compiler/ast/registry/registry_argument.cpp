//===-- registry_argument.cpp -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#include "compiler/ast/registry/registry_argument.h"
#include <llvm-18/llvm/IR/Value.h>
#include <vector>

RegistryArgument::RegistryArgument()
= default;

void RegistryArgument::add(llvm::Value* arg)
{
    _args.push_back(arg);
}

auto RegistryArgument::get() -> std::vector<llvm::Value*>&
{
    return _args;
}

void RegistryArgument::clear()
{
    _args.clear();
}



