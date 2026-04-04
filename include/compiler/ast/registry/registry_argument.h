//===-- registry_argument.h -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#ifndef CCD2D336_FEA8_4097_8361_E253E2A1166E
#define CCD2D336_FEA8_4097_8361_E253E2A1166E
#include <llvm-18/llvm/IR/Value.h>
#include <vector>

class RegistryArgument
{
private: 
    std::vector<llvm::Value*> _args;

public: 
   RegistryArgument();
   void add(llvm::Value* arg);
   auto get() -> std::vector<llvm::Value*>&;
   void clear();
};

#endif /* CCD2D336_FEA8_4097_8361_E253E2A1166E */
