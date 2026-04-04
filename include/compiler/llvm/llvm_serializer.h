//===-- llvm_serializer.h -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#ifndef LLVMSERIALIZER_H
#define LLVMSERIALIZER_H

#include <llvm-18/llvm/Support/raw_ostream.h>
#include "llvm/IR/Module.h"
#include "llvm/IR/LLVMContext.h"
#include <string>
#include <system_error>

using namespace llvm;
using namespace std;

class LlvmSerializer {
    private: 
        Module* _module;
        error_code errorCode;
            
    public: 
        explicit LlvmSerializer(Module& mod) : _module(&mod) {}
        void SaveLLVMCode(const string& path);
};

#endif /* LLVMSERIALIZER_H */
