#include "compiler/ast/registry/stack/registry_variable.h"
#include "compiler/visitor/code_gen/visitor_general_gen_code.h"
#include "compiler/ast/ast_genere.h"
#include "compiler/llvm/gestion_variable.h"
#include <llvm-18/llvm/IR/Value.h>

void GeneralVisitorGenCode::visiter(NodeAssignmentVariable* nodeAssignmentVariable)
{
    llvm::Value* expressionResult = evaluateExpression(nodeAssignmentVariable->getExpression()).getAddress();

    VariableAddressor addressor(_contextGenCode);
    Symbol symbol = addressor.getAddress(nodeAssignmentVariable->getToken().value);
    
    llvm::Value* existingVariable = symbol.getAddress();
    
    llvm::Type* variableTypeLLVM = nullptr;
    if (symbol.getType() != nullptr) {
        variableTypeLLVM = symbol.getType()->generateLLVMType(_contextGenCode->getBackend()->getContext());
    }
    
    VariableAssigner assigner(_contextGenCode);
    assigner.assign(existingVariable, expressionResult, variableTypeLLVM);
    
    // Reset the value to avoid problems
    _contextGenCode->setTemporaryValue(Symbol(nullptr, _contextGenCode->getTemporaryValue().getType(), _contextGenCode->getTemporaryValue().getPointedElementType()));
}

