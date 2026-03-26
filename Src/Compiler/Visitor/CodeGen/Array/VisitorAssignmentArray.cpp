#include "Compiler/AST/Registry/Stack/RegistryVariable.h"
#include "Compiler/AST/Registry/Types/IType.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include "Compiler/Visitor/CodeGen/VisitorGeneralGenCode.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Registry/Types/TypeArray.h"
#include "Compiler/LLVM/GestionVariable.h"
#include "Compiler/Utils/PrysmaCast.h"
#include <cstddef>
#include <llvm-18/llvm/IR/Value.h>
#include <llvm/Support/Casting.h>
#include <string>
#include <vector>

void GeneralVisitorGenCode::visiter(NodeAssignmentArray* nodeAssignmentArray)
{
    // Evaluate the index expression
    llvm::Value* indexValue = evaluateExpression(nodeAssignmentArray->getExpressionIndex()).getAddress();
  
    // Evaluate the expression to assign
    llvm::Value* expressionResult = evaluateExpression(nodeAssignmentArray->getExpression()).getAddress();

    // Retrieve the array
    std::string arrayNameStr = nodeAssignmentArray->getToken().value;
    Symbol symbol;
    llvm::Value* value = nullptr;

    if (arrayNameStr.find('.') != std::string::npos) {
        size_t pos = arrayNameStr.find('.');
        std::string objectName = arrayNameStr.substr(0, pos);
        std::string attributeName = arrayNameStr.substr(pos + 1);

        VariableLoader loader(_contextGenCode);
        Symbol objectSymbol = loader.load(objectName);
        llvm::Value* object = objectSymbol.getAddress();

        std::string className = getClassNameFromSymbol(objectSymbol);
        auto* classInfo = _contextGenCode->getRegistryClass()->get(className).get();
        auto iterator = classInfo->getMemberIndices().find(attributeName);
        unsigned int indexObj = iterator->second;

        Token tokenAttribute; tokenAttribute.value = attributeName; tokenAttribute.type = TOKEN_IDENTIFIER;
        symbol = classInfo->getRegistryVariable()->getVariable(tokenAttribute);
        
        llvm::Type* structType = classInfo->getStructType();
        value = _contextGenCode->getBackend()->getBuilder().CreateStructGEP(structType, object, indexObj, objectName + "_" + attributeName + "_ptr");
    } else {
        VariableAddressor addressor(_contextGenCode);
        symbol = addressor.getAddress(arrayNameStr);
        value = symbol.getAddress();
    }

    // Assignment to an array element with the index
    std::vector<llvm::Value*> indices = { 
        _contextGenCode->getBackend()->getBuilder().getInt32(0), 
        indexValue 
    };
    
    // Cannot retrieve the array size at compile time if the index is a variable.
    // Must retrieve the array type directly from the allocation instruction.
    llvm::Type* arrayType = nullptr;
    if (symbol.getType() != nullptr) {
        arrayType = symbol.getType()->generateLLVMType(_contextGenCode->getBackend()->getContext());
    } else {
        auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(value);
        if (allocaInst != nullptr) {
            arrayType = allocaInst->getAllocatedType();
        }
    }

    llvm::Value* ptrElement = nullptr;
    llvm::Type* elementType = nullptr;

    if (arrayType->isArrayTy()) {
        ptrElement = _contextGenCode->getBackend()->getBuilder().CreateGEP(
            arrayType, 
            value, 
            indices
        );
        elementType = arrayType->getArrayElementType();
    } else {
        // It's a pointer to an array (reference), must load the pointer
        llvm::Value* loadedPtr = _contextGenCode->getBackend()->getBuilder().CreateLoad(arrayType, value);
        
        IType* astType = symbol.getType();
        auto* declaredArrayType = prysma::dyn_cast<TypeArray>(astType);
        if (declaredArrayType != nullptr) {
            elementType = declaredArrayType->getChildType()->generateLLVMType(_contextGenCode->getBackend()->getContext());
        } else {
            elementType = symbol.getPointedElementType();
        }
        
        ptrElement = _contextGenCode->getBackend()->getBuilder().CreateGEP(
            elementType, 
            loadedPtr, 
            indexValue
        );
    }

    llvm::Value* castedValue = _contextGenCode->getBackend()->createAutoCast(expressionResult, elementType);
    
    _contextGenCode->getBackend()->getBuilder().CreateStore(castedValue, ptrElement);
    
    // Reset the value to avoid problems
    _contextGenCode->setTemporaryValue(Symbol(nullptr, _contextGenCode->getTemporaryValue().getType(), _contextGenCode->getTemporaryValue().getPointedElementType()));
}