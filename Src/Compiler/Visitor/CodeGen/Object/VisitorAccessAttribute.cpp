#include "Compiler/AST/Registry/Stack/RegistryVariable.h"
#include "Compiler/Visitor/CodeGen/VisitorGeneralGenCode.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/LLVM/GestionVariable.h"
#include "Compiler/AST/Registry/RegistryClass.h"
#include "Compiler/Visitor/CodeGen/Helper/ErrorHelper.h"
#include <llvm/IR/Instructions.h>
#include <string>

void GeneralVisitorGenCode::visiter(NodeAccesAttribute* nodeAccessAttribute)
{
    std::string objectName = nodeAccessAttribute->getNomObject().value;
    std::string attributeName = nodeAccessAttribute->getNomAttribute().value;

    VariableLoader loader(_contextGenCode);
    Symbol objectSymbol = loader.load(objectName);
    llvm::Value* object = objectSymbol.getAddress();

    std::string className = getClassNameFromSymbol(objectSymbol);

    if (className.empty()) {
        ErrorHelper::compilationError("Unable to determine the class of object '" + objectName + "'");
    }

    auto* classInfo = _contextGenCode->getRegistryClass()->get(className).get();
    classInfo = ErrorHelper::verifyNotNull(classInfo, "Class '" + className + "' not found in the registry");

    if (!classInfo->getRegistryVariable()->variableExists(attributeName)) {
        ErrorHelper::compilationError("Attribute '" + attributeName + "' does not exist in class '" + className + "'");
    }

    auto iterator = classInfo->getMemberIndices().find(attributeName);
    if (iterator == classInfo->getMemberIndices().end()) {
        ErrorHelper::compilationError("Attribute '" + attributeName + "' has no index in " + className);
    }
    
    unsigned int index = iterator->second;

    auto& builder = _contextGenCode->getBackend()->getBuilder();

    Symbol varSymbol = classInfo->getRegistryVariable()->getVariable(nodeAccessAttribute->getNomAttribute());
    
    llvm::Type* structType = classInfo->getStructType();

    llvm::Value* attributePtr = builder.CreateStructGEP(structType, object, index, objectName + "_" + attributeName + "_ptr");

    llvm::Value* attributeValue = builder.CreateLoad(
        varSymbol.getType()->generateLLVMType(_contextGenCode->getBackend()->getContext()), 
        attributePtr, 
        objectName + "_" + attributeName
    );

    _contextGenCode->setTemporaryValue(Symbol(attributeValue, varSymbol.getType(), nullptr));
}
