#include "Compiler/AST/Utils/BuilderEnvironmentRegistryVariable.h"
#include "Compiler/AST/Registry/ContextGenCode.h"
#include "Compiler/AST/Registry/Stack/RegistryVariable.h"
#include "Compiler/AST/Registry/Types/IType.h"
#include "Compiler/LLVM/LlvmBackend.h"
#include "Compiler/Lexer/Lexer.h"
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