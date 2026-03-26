#include "Compiler/Visitor/CodeGen/VisitorGeneralGenCode.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Registry/Types/TypeSimple.h"
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LLVMContext.h>
#include "Compiler/Visitor/CodeGen/Helper/ErrorHelper.h"

void GeneralVisitorGenCode::visiter(NodeNegation* node)
{
    // Safety check
    if (node == nullptr || node->getOperande() == nullptr) {
        ErrorHelper::compilationError("NodeNegation or operand invalid");
    }
    
    // Evaluate the operand (must be boolean)
    node->getOperande()->accept(this);
    llvm::Value* operandVal = _contextGenCode->getTemporaryValue().getAddress();

    auto& builder = _contextGenCode->getBackend()->getBuilder();
    llvm::LLVMContext& context = _contextGenCode->getBackend()->getContext();
    
    // Ensure the operand is of type i1 (bool)
    if (!operandVal->getType()->isIntegerTy(1)) {
        operandVal = builder.CreateTrunc(operandVal, llvm::Type::getInt1Ty(context), "tobool");
    }
    
    // Create logical negation with NOT
    llvm::Value* result = builder.CreateNot(operandVal, "not");

    _contextGenCode->setTemporaryValue(Symbol(result, _contextGenCode->getTemporaryValue().getType(), _contextGenCode->getTemporaryValue().getPointedElementType()));
    _contextGenCode->setTemporaryValue(Symbol(_contextGenCode->getTemporaryValue().getAddress(), new (_contextGenCode->getArena()->Allocate<TypeSimple>()) TypeSimple(llvm::Type::getInt1Ty(context)), _contextGenCode->getTemporaryValue().getPointedElementType()));
}
