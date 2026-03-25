#include "Compiler/AST/Registry/Stack/RegistryVariable.h"
#include "Compiler/Visitor/CodeGen/VisitorGeneralGenCode.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Registry/Types/IType.h"
#include <llvm-18/llvm/IR/Value.h>

void GeneralVisitorGenCode::visiter(NodeReturn* nodeReturn)
{
    nodeReturn->getValeurReturn()->accept(this);
    llvm::Value* valeurEvaluee = _contextGenCode->getValeurTemporaire().getAdresse();
    IType* typeReturnObj = _contextGenCode->getReturnContextCompilation()->recupererContext();
    llvm::Type* typeReturnLLVM = typeReturnObj->generatedrTypeLLVM(_contextGenCode->getBackend()->getContext());
    llvm::Value* valeurReturn = _contextGenCode->getBackend()->creerAutoCast(valeurEvaluee, typeReturnLLVM);
    _contextGenCode->getBackend()->getBuilder().CreateRet(valeurReturn);
    _contextGenCode->modifierValeurTemporaire(Symbole(valeurReturn, _contextGenCode->getValeurTemporaire().getType(), _contextGenCode->getValeurTemporaire().getTypePointeElement()));
}