//===-- visitor_call_object.cpp ---------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#include "compiler/ast/registry/stack/registry_variable.h"
#include "compiler/ast/registry/registry_function.h"
#include "compiler/visitor/code_gen/visitor_general_gen_code.h"
#include "compiler/ast/ast_genere.h"
#include "compiler/ast/registry/registry_class.h"
#include "compiler/visitor/code_gen/helper/error_helper.h"
#include "compiler/visitor/code_gen/helper/v_table_navigator.h"
#include "compiler/utils/prysma_cast.h"
#include <cstdint>
#include <llvm-18/llvm/IR/Value.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/Casting.h>
#include <string>
#include <vector>

/*
                                                                                                                        
                                                          class Dog implements IAnimal                                  
                                                          {                                                             
                                                              dec int32 age = 5;                                        
                                                              fn void Dog()                                             
                                                              {                                                         
                                                                                                                        
                                                              }                                                         
                                                                                                                        
                                                              fn void makeSound()                                       
                                                              {                                                         
                                                                  call print("Woof!");                                  
                                                              }                                                         
                                                          }                                                             
                                                                                                                        
                                                                                        Struct                          
                                                                                                                        
                                                                          0        1        2         3        4        
                                        points to the data structure  ┌────────┬────────┬────────┬────────┬────────┐    
                            vPtr                      ┌───────────────►        │        │        │        │        │    
   ┌─────────────────────────────────┐                │               │ Memory │ Memory │ Memory │Memory  │Memory  │    
   │                                 │                │               └──┬─────┴───┬────┴────────┴────────┴────────┘    
   │ dec IAnimal dog = new Dog();    ┼────────────────┘                  │         │                                    
   │                                 │                                   │         └───────────────┐                    
   └─────────────────────────────────┘                          Points to│the vtable               ▼                    
                                                                         │                 dec int32 age = 5;           
      // The vtable contract is managed by the IAnimal interface         │                                              
                                                                         │                                              
         Interface IAnimal                                               │0        1        2         3        4        
         {                                                            ┌──▼─────┬────────┬────────┬────────┬────────┐    
             fn void makeSound(); ──────────────────────────►         │        │        │        │        │        │    
         }                                                     vtable │ Memory │Memory  │Memory  │Memory  │Memory  │    
                                                                      └───┬────┴────┬───┴────────┴────────┴────────┘    
                                                                          │         │                                   
                                                                          │         │                                   
                                                                          │         │                                   
                                                                          │         │                                   
                                                                          ▼         │                                   
                                                                     fn void Dog()  │                                   
                                                                         {          │                                   
                                                                                    ▼                                   
                                                                         }        fn void makeSound()                   
                                                                                     {                                  
                                                                                         call print("Woof!");           
                                                                                     }                                  
                                                                                                                        
                                                                                                                        
*/


void GeneralVisitorGenCode::visiter(NodeCallObject* nodeCallObject)
{
    llvm::StringRef methodName = nodeCallObject->getNomMethode().value;

    // Receiver evaluation (e.g., dog.getHead().bark())
    INode* receiver = nodeCallObject->getReceiver();
    receiver->accept(this);
    
    Symbol objectSymbol = _contextGenCode->getTemporaryValue();
    llvm::Value* object = objectSymbol.getAddress();

    // If it is an alloca, we load the instance
    if (auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(object)) {
        object = _contextGenCode->getBackend()->getBuilder().CreateLoad(allocaInst->getAllocatedType(), object);
    }

    std::string className = getClassNameFromSymbol(objectSymbol);

    if (className.empty()) {
        ErrorHelper::compilationError("Class undetermined for object executing method '" + methodName.str() + "'");
    }

    auto* classInfo = _contextGenCode->getRegistryClass()->get(className).get();
    classInfo = ErrorHelper::verifyNotNull(classInfo, "Class '" + className + "' not found");
    
    // We retrieve the StructType directly from classInfo rather than probing the pointer
    llvm::StructType* structType = classInfo->getStructType();
    
    // Access to the vptr (always at index 0 according to your memory layout)
    llvm::Value* vptrAddress = _contextGenCode->getBackend()->getBuilder().CreateStructGEP(structType, object, 0, "vptr_address");

    // Loading the vtable
    llvm::Value* vtable = _contextGenCode->getBackend()->getBuilder().CreateLoad(_contextGenCode->getBackend()->getBuilder().getPtrTy(), vptrAddress, "vptr");
    
    // Method index
    uint32_t methodIndex = classInfo->getMethodIndex(methodName.str());

    ErrorHelper::verifyExistence(*classInfo->getRegistryFunctionLocal(), methodName.str(),
        "Method '" + methodName.str() + "' does not exist in '" + className + "'");

    auto& builder = _contextGenCode->getBackend()->getBuilder();
    VTableNavigator navigator(&builder);
    
    llvm::Value* methodPointer = navigator.getMethodPointer(vtable, classInfo->getVTable()->getValueType(), methodIndex);
    
    // Argument preparation (the first one is 'this')
    std::vector<llvm::Value*> args;
    args.push_back(object);

    const auto& symbolPtr = classInfo->getRegistryFunctionLocal()->get(methodName.str());
    const auto* functionSymbol = prysma::cast<const SymbolFunctionLocal>(symbolPtr.get());
    llvm::FunctionType* functionType = functionSymbol->function->getFunctionType();

    unsigned int paramIndex = 1; 
    for (INode* argumentChild : nodeCallObject->getChildren()) 
    {
        argumentChild->accept(this);
        llvm::Value* argumentValue = _contextGenCode->getTemporaryValue().getAddress();
        argumentValue = ErrorHelper::verifyNotNull(argumentValue, "Object call argument did not generate a value");

        llvm::Value* finalValue = argumentValue;
        if (paramIndex < functionType->getNumParams()) {
            llvm::Type* expectedType = functionType->getParamType(paramIndex);
            finalValue = _contextGenCode->getBackend()->createAutoCast(argumentValue, expectedType);
        }
        args.push_back(finalValue);
        paramIndex++;
    }

    // Actual call
    if(methodName == className) {
        builder.CreateCall(functionSymbol->function, args);
        return;
    }

    if (functionSymbol->function->getReturnType()->isVoidTy()) { 
        builder.CreateCall(functionType, methodPointer, args);
        _contextGenCode->setTemporaryValue(Symbol(nullptr, nullptr, nullptr));
    } else {
        llvm::Value* result = builder.CreateCall(functionType, methodPointer, args, "object_call_result");
        _contextGenCode->setTemporaryValue(Symbol(result, functionSymbol->returnType, nullptr));
    }
}
