#include "compiler/ast/ast_genere.h"
#include "compiler/ast/registry/node_component_registry.h"
#include "compiler/ast/registry/stack/registry_variable.h"
#include "compiler/visitor/code_gen/visitor_general_gen_code.h"
#include "compiler/ast/registry/types/i_type.h"
#include <llvm-18/llvm/IR/Value.h>

void GeneralVisitorGenCode::visiter(NodeReturn* nodeReturn)
{
    auto& nodeData = _contextGenCode->getNodeComponentRegistry()->get<NodeReturnComponents>(nodeReturn->getNodeId());

    nodeData.getReturnValue()->accept(this);

    llvm::Value* evaluatedValue = _contextGenCode->getTemporaryValue().getAddress();
    IType* returnTypeObj = _contextGenCode->getReturnContextCompilation()->getContext();
    llvm::Type* returnTypeLLVM = returnTypeObj->generateLLVMType(_contextGenCode->getBackend()->getContext());
    llvm::Value* returnValue = _contextGenCode->getBackend()->createAutoCast(evaluatedValue, returnTypeLLVM);

    _contextGenCode->getBackend()->getBuilder().CreateRet(returnValue);
    _contextGenCode->setTemporaryValue(Symbol(returnValue, _contextGenCode->getTemporaryValue().getType(), _contextGenCode->getTemporaryValue().getPointedElementType()));
}

/*il faut que je return des références car sinon, le user peut littéralement modifier les ptr présents dans les sparse sets, ce qui est vraiment bad.*/