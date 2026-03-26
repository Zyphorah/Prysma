#include "Compiler/AST/Registry/Stack/RegistryVariable.h"
#include "Compiler/Visitor/CodeGen/VisitorGeneralGenCode.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Registry/Types/IType.h"
#include <llvm-18/llvm/IR/Value.h>

void GeneralVisitorGenCode::visiter(NodeReturn* nodeReturn)
{
    nodeReturn->getValeurReturn()->accept(this);
    llvm::Value* evaluatedValue = _contextGenCode->getTemporaryValue().getAddress();
    IType* returnTypeObj = _contextGenCode->getReturnContextCompilation()->getContext();
    llvm::Type* returnTypeLLVM = returnTypeObj->generateLLVMType(_contextGenCode->getBackend()->getContext());
    llvm::Value* returnValue = _contextGenCode->getBackend()->createAutoCast(evaluatedValue, returnTypeLLVM);
    _contextGenCode->getBackend()->getBuilder().CreateRet(returnValue);
    _contextGenCode->setTemporaryValue(Symbol(returnValue, _contextGenCode->getTemporaryValue().getType(), _contextGenCode->getTemporaryValue().getPointedElementType()));
}