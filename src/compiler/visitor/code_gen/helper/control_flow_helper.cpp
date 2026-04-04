//===-- control_flow_helper.cpp -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#include "compiler/visitor/code_gen/helper/control_flow_helper.h"
#include <llvm-18/llvm/IR/BasicBlock.h>
#include <string>

auto ControlFlowHelper::createControlBlocks(
    llvm::Function* function,
    llvm::LLVMContext& context,
    const std::string& inputName,
    const std::string& bodyName,
    const std::string& outputName
) -> ControlBlocks {
    return {
        llvm::BasicBlock::Create(context, inputName, function),
        llvm::BasicBlock::Create(context, bodyName, function),
        llvm::BasicBlock::Create(context, outputName, function)
    };
}
