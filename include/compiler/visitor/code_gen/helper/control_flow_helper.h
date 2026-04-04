//===-- control_flow_helper.h -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#ifndef C6E13746_23A9_489F_AD56_E4937A64E42F
#define C6E13746_23A9_489F_AD56_E4937A64E42F

#include <llvm-18/llvm/IR/BasicBlock.h>
#include <llvm-18/llvm/IR/Function.h>
#include <llvm-18/llvm/IR/LLVMContext.h>
#include <string>

// Structure representing control flow blocks
struct ControlBlocks {
    llvm::BasicBlock* input;
    llvm::BasicBlock* body; 
    llvm::BasicBlock* output;
};

class ControlFlowHelper {
public:
    // Creates control flow blocks for a function
    static auto createControlBlocks(
        llvm::Function* function,
        llvm::LLVMContext& context,
        const std::string& inputName,
        const std::string& bodyName, 
        const std::string& outputName
    ) -> ControlBlocks;
};

#endif /* C6E13746_23A9_489F_AD56_E4937A64E42F */
