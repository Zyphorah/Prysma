//===-- builder_environment_registry_variable.cpp -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#include "compiler/ast/utils/builder_environment_registry_variable.h"
#include "compiler/ast/registry/context_gen_code.h"
#include "compiler/ast/registry/stack/registry_variable.h"
#include "compiler/ast/registry/types/i_type.h"
#include "compiler/llvm/llvm_backend.h"
#include "compiler/lexer/lexer.h"
#include "llvm/IR/GlobalVariable.h"
#include <llvm/IR/GlobalValue.h>
#include <llvm/IR/Type.h>

BuilderEnvironmentRegistryVariable::BuilderEnvironmentRegistryVariable(ContextGenCode* contextGenCode)
    : _contextGenCode(contextGenCode) {}

BuilderEnvironmentRegistryVariable::~BuilderEnvironmentRegistryVariable() {}

// Currently unused, for the future, another development iteration, it exists for now but is not yet tested
// A bonus feature
// This class is intended to fill the global variable registry located outside a function
// example : 
// scope {
//     dec int64 a = 10;
//     dec string b = "string";
//     fn void test()
//     {
//     }
// }

void BuilderEnvironmentRegistryVariable::fill()
{   
    auto globalVariablesPass1 = _contextGenCode->getRegistryVariable()->getGlobalVariables();

    _contextGenCode->getRegistryVariable()->clearTop(); 

    for(auto const& [name, symbol] : globalVariablesPass1)
    {
        llvm::Type* llvmType = symbol.getType()->generateLLVMType(_contextGenCode->getBackend()->getContext());
        
        auto* globalVariable = new llvm::GlobalVariable(
            _contextGenCode->getBackend()->getModule(),
            llvmType,
            false,
            llvm::GlobalValue::ExternalLinkage,
            nullptr,
            name
        );

        Token token;
        token.value = name;
        
        _contextGenCode->getRegistryVariable()->registerVariable(token, Symbol(globalVariable, symbol.getType()));
    }
}
