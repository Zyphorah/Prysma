#include "Compiler/AST/Registry/Stack/RegistryVariable.h"
#include "Compiler/AST/Registry/Types/IType.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include "Compiler/Visitor/CodeGen/VisitorGeneralGenCode.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Registry/Types/TypeSimple.h"
#include "Compiler/AST/Registry/Types/TypeArray.h"
#include "Compiler/LLVM/GestionVariable.h"
#include "Compiler/Utils/PrysmaCast.h"
#include "Compiler/Visitor/CodeGen/Helper/ErrorHelper.h"
#include <cstddef>
#include <llvm-18/llvm/IR/Instructions.h>
#include <llvm-18/llvm/Support/Casting.h>
#include <string>

void GeneralVisitorGenCode::visiter(NodeReadingArray* nodeReadingArray)
{
    std::string arrayNameStr = nodeReadingArray->getNomArray().value;
    Symbol symbol;
    llvm::Value* arrayAddress = nullptr;

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
        arrayAddress = _contextGenCode->getBackend()->getBuilder().CreateStructGEP(structType, object, indexObj, objectName + "_" + attributeName + "_ptr");
    } else {
        VariableAddressor addressor(_contextGenCode);
        symbol = addressor.getAddress(arrayNameStr);
        arrayAddress = symbol.getAddress();
    }

    llvm::Type* arrayType = nullptr;
    if (symbol.getType() != nullptr) {
        arrayType = symbol.getType()->generateLLVMType(_contextGenCode->getBackend()->getContext());
    } else {
        auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(arrayAddress);
        allocaInst = ErrorHelper::verifyNotNull(allocaInst, "The array address is not an allocation instruction");
        arrayType = allocaInst->getAllocatedType();
    }
    
    // Compute the index from the equation 
    nodeReadingArray->getIndexEquation()->accept(this);
    llvm::Value* indexValue = _contextGenCode->getTemporaryValue().getAddress();
    
    llvm::Value* elementAddress = nullptr;
    llvm::Type* elementType = nullptr;

    if (arrayType->isArrayTy()) {
        // compute the offset with GEP 
        elementAddress = _contextGenCode->getBackend()->getBuilder().CreateGEP(arrayType, arrayAddress, 
            { 
                _contextGenCode->getBackend()->getBuilder().getInt32(0), 
                indexValue 
            }, nodeReadingArray->getNomArray().value);
        elementType = arrayType->getArrayElementType();
    } else {
        // It's a pointer to an array (reference), must load the pointer
        llvm::Value* loadedPtr = _contextGenCode->getBackend()->getBuilder().CreateLoad(arrayType, arrayAddress);
        
        IType* astType = symbol.getType();
        auto* declaredArrayType = prysma::dyn_cast<TypeArray>(astType);
        if (declaredArrayType != nullptr) {
            elementType = declaredArrayType->getChildType()->generateLLVMType(_contextGenCode->getBackend()->getContext());
        } else {
            elementType = symbol.getPointedElementType();
        }
        
        elementAddress = _contextGenCode->getBackend()->getBuilder().CreateGEP(
            elementType, 
            loadedPtr, 
            indexValue,
            nodeReadingArray->getNomArray().value
        );
    }
        
    // Load the value from the element address
    llvm::Value* elementValue = _contextGenCode->getBackend()->getBuilder().CreateLoad(elementType, elementAddress, nodeReadingArray->getNomArray().value);

    _contextGenCode->setTemporaryValue(Symbol(elementValue, _contextGenCode->getTemporaryValue().getType(), _contextGenCode->getTemporaryValue().getPointedElementType()));
    _contextGenCode->setTemporaryValue(Symbol(_contextGenCode->getTemporaryValue().getAddress(), new (_contextGenCode->getArena()->Allocate<TypeSimple>()) TypeSimple(elementType), _contextGenCode->getTemporaryValue().getPointedElementType()));
}

