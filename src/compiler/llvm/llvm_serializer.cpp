//===-- llvm_serializer.cpp -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#include "compiler/llvm/llvm_serializer.h"
#include <llvm-18/llvm/Support/raw_ostream.h>
#include <string>

void LlvmSerializer::SaveLLVMCode(const string& path)
{
        llvm::raw_fd_ostream out(path, errorCode);
        _module->print(out, nullptr);
        out.close();
}
