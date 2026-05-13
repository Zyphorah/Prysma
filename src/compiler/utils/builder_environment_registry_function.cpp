//===-- builder_environment_registry_function.cpp ---------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#include "compiler/ast/utils/builder_environment_registry_function.h"
#include "compiler/ast/registry/context_gen_code.h"
#include "compiler/ast/registry/registry_function.h"
#include "compiler/ast/registry/registry_class.h"
#include "compiler/ast/ast_genere.h"
#include "compiler/utils/prysma_cast.h"
#include "compiler/visitor/extractors/members_extractor_class.h"
#include <llvm-18/llvm/IR/Function.h>
#include <llvm-18/llvm/IR/GlobalVariable.h>
#include <llvm/ADT/StringRef.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/GlobalValue.h>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

BuilderEnvironmentRegistryFunction::BuilderEnvironmentRegistryFunction(ContextGenCode* contextGenCode)
    : _contextGenCode(contextGenCode) {}

BuilderEnvironmentRegistryFunction::~BuilderEnvironmentRegistryFunction() = default;

auto BuilderEnvironmentRegistryFunction::alignVTableWithParent(Class* classInfo, const std::vector<NodeDeclarationFunction*>& parentMethodList) -> std::vector<llvm::Constant*>
{
     // Build the vtable by aligning the parent's methods at the same positions
    std::vector<llvm::Constant*> vtableElements;
    
    // Add the parent's function pointers to the vtable, in the same order
    for (NodeDeclarationFunction* parentMethodDecl : parentMethodList) {
        llvm::StringRef parentMethodName = parentMethodDecl->getNom().value;
        std::string parentMethodNameStr = parentMethodName.str();
        
        // Search for the corresponding method in the current class
        llvm::Function* functionImpl = nullptr;
        if (classInfo->getMaterializedFunctionRegistry()->exists(parentMethodNameStr)) {
            const auto& symbol = classInfo->getMaterializedFunctionRegistry()->get(parentMethodNameStr);
            if (!prysma::isa<SymbolFunctionLocal>(symbol.get())) {
                throw std::runtime_error("Error: Expected SymbolFunctionLocal");
            }
            auto* localSymbol = prysma::cast<SymbolFunctionLocal>(symbol.get());
            functionImpl = localSymbol->getFunction();
        }
        
        // If found, add the function pointer to the vtable
        if (functionImpl != nullptr) {
            vtableElements.push_back(llvm::ConstantExpr::getBitCast(
                functionImpl,
                llvm::PointerType::get(llvm::Type::getInt8Ty(_contextGenCode->getBackend()->getContext()), 0)
            ));
            classInfo->getMethodIndices()[parentMethodName.str()] = static_cast<unsigned int>(vtableElements.size() - 1);
        }
    }
    return vtableElements;
}

auto BuilderEnvironmentRegistryFunction::checkIsInTheParent(llvm::StringRef methodName, const std::vector<NodeDeclarationFunction*>& parentMethodList) -> bool
{
  for (NodeDeclarationFunction* parentMethodDecl : parentMethodList) {
        if (parentMethodDecl->getNom().value == methodName) {
            return true;
            break;
        }
    }  
    return false;
}

void BuilderEnvironmentRegistryFunction::addAtTheEndOfVtable(llvm::StringRef methodName, Class* classInfo, std::vector<llvm::Constant*>& vtableElements)
{
    const auto& symbol = classInfo->getMaterializedFunctionRegistry()->get(methodName);
    if (!prysma::isa<SymbolFunctionLocal>(symbol.get())) {
        throw std::runtime_error("Error: Expected SymbolFunctionLocal");
    }
    auto* localSymbol = prysma::cast<SymbolFunctionLocal>(symbol.get()); 
    if (localSymbol->getFunction() != nullptr) {
        vtableElements.push_back(llvm::ConstantExpr::getBitCast(
            localSymbol->getFunction(),
            llvm::PointerType::get(llvm::Type::getInt8Ty(_contextGenCode->getBackend()->getContext()), 0)
        ));
        classInfo->getMethodIndices()[std::string(methodName)] = static_cast<unsigned int>(vtableElements.size() - 1);
    }
}

void BuilderEnvironmentRegistryFunction::appendNewMethodsToVTable(std::vector<llvm::Constant*>& vtableElements, Class* classInfo, const std::string& className, const std::vector<NodeDeclarationFunction*>& parentMethodList)
{
    // Add additional methods from the current class that are not in the parent
    auto methodKeys = classInfo->getMaterializedFunctionRegistry()->getKeys();
    for (const auto& key : methodKeys) {
        llvm::StringRef methodName = key;
        // Skip the builder (constructor) — it is not part of the vtable
        if (methodName == className) {
            continue;
        }
        bool isInParent = checkIsInTheParent(methodName, parentMethodList);
        
        // If not a parent method, add it at the end of the vtable
        if (!isInParent) {
           addAtTheEndOfVtable(methodName,classInfo,vtableElements);
        }
    }
}

void BuilderEnvironmentRegistryFunction::buildVTable(Class* classInfo, const std::string& className, const std::vector<NodeDeclarationFunction*>& parentMethodList)
{
    // Build the vtable by aligning the parent's methods at the same positions
    std::vector<llvm::Constant*> vtableElements = alignVTableWithParent(classInfo,parentMethodList);
    appendNewMethodsToVTable(vtableElements,classInfo,className,parentMethodList);

    // Create a constant array for the vtable
    llvm::ArrayType* vtableType = llvm::ArrayType::get(
        llvm::PointerType::get(llvm::Type::getInt8Ty(_contextGenCode->getBackend()->getContext()), 0),
        vtableElements.size()
    );
    llvm::Constant* vtableInitializer = llvm::ConstantArray::get(vtableType, vtableElements);
    
    // Create a global variable for the vtable
    llvm::Module* module = &(_contextGenCode->getBackend()->getModule());
    auto* vtableGlobal = new llvm::GlobalVariable(
        *module,
        vtableType,
        true,  
        llvm::GlobalValue::LinkageTypes::InternalLinkage,
        vtableInitializer,
        "vtable_" + className
    );
    classInfo->setVTable(vtableGlobal);
}

auto BuilderEnvironmentRegistryFunction::fillArgFunction(SymbolFunctionGlobal* oldSymbol) -> std::vector<llvm::Type*>
{
    std::vector<llvm::Type*> paramTypes;
    auto* node = oldSymbol->getNode();
    if (node == nullptr) {
        return paramTypes;
    }
    for (auto* arg : node->getArguments())
    {
        auto* argFunction = prysma::cast<NodeArgFunction>(arg);
        paramTypes.push_back(argFunction->getType()->generateLLVMType(_contextGenCode->getBackend()->getContext()));
    }
    return paramTypes;
}

void BuilderEnvironmentRegistryFunction::projectGlobalToMaterialized()
{
    for(const auto& key : _contextGenCode->getRegistryFunctionGlobal()->getKeys())
    {
        // Skip if already materialized (e.g. standard functions like print, backSlashN)
        if (_contextGenCode->getMaterializedFunctionRegistry()->exists(key)) {
            continue;
        }

        const auto& oldSymbolUniquePtr = _contextGenCode->getRegistryFunctionGlobal()->get(key);
        auto* oldSymbol = prysma::cast<SymbolFunctionGlobal>(oldSymbolUniquePtr.get());
        
        llvm::Type* retType = oldSymbol->getReturnType()->generateLLVMType(_contextGenCode->getBackend()->getContext());
        
        std::vector<llvm::Type*> paramTypes = fillArgFunction(oldSymbol);
        
        llvm::FunctionType* funcType = llvm::FunctionType::get(retType, paramTypes, false);
        
        llvm::Function* realFunction = llvm::Function::Create(
            funcType, 
            llvm::Function::ExternalLinkage, 
            key,          
            _contextGenCode->getBackend()->getModule()
        );

        // This is where we create the function in the local function registry
        auto newSymbol = std::make_unique<SymbolFunctionLocal>(realFunction,oldSymbol->getReturnType(),oldSymbol->getNode());
        _contextGenCode->getMaterializedFunctionRegistry()->registerElement(key, std::move(newSymbol));        
    }
}

auto BuilderEnvironmentRegistryFunction::fillClassMethodsParamTypes(const SymbolFunctionLocal* symbol) -> std::vector<llvm::Type*>
{
    std::vector<llvm::Type*> paramTypes;
    // Add the hidden 'this' parameter as the first parameter
    paramTypes.push_back(llvm::PointerType::getUnqual(_contextGenCode->getBackend()->getContext()));

    for (auto* arg : symbol->getNode()->getArguments()) {
        auto* argFunction = prysma::cast<NodeArgFunction>(arg);
        paramTypes.push_back(argFunction->getType()->generateLLVMType(_contextGenCode->getBackend()->getContext()));
    }
    return paramTypes;
}

void BuilderEnvironmentRegistryFunction::fillClassMethods(const std::basic_string<char>&  className)
{
    auto const& classInfo = _contextGenCode->getRegistryClassGlobal()->get(className);
    
    // Check if the class contains a builder, otherwise the class is not valid.
    if (!classInfo->getMaterializedFunctionRegistry()->exists(className)) {
        throw std::runtime_error("Error: The class '" + className + "' must have a builder with the same name");
    }
    
    // Build the vtable for this class (parent methods alignment)
    std::vector<NodeDeclarationFunction*> parentMethodList;
    if (classInfo->getParentInheritance() != nullptr) {
        MembersExtractorClass parentExtractor;
        classInfo->getParentInheritance()->accept(&parentExtractor);
        parentMethodList = parentExtractor.getMethods();
    }
        
    for (const auto& methodName : classInfo->getMaterializedFunctionRegistry()->getKeys())
    {
        // Retrieve the SymbolFunctionLocal created in FillingVisitorRegistry
        const auto& symbolUniquePtr = classInfo->getMaterializedFunctionRegistry()->get(methodName);
        auto* symbol = prysma::cast<SymbolFunctionLocal>(symbolUniquePtr.get());
        
        if (symbol->getNode() == nullptr) 
        {
            std::string errorMsg = "Error: SymbolFunctionLocal for method '";
            errorMsg += methodName;
            errorMsg += "' of class '";
            errorMsg += className;
            errorMsg += "' has a null node";
            throw std::runtime_error(errorMsg);
        }

        // If already processed by a previous unit, reset so we can recreate in the current module.
        // Pass 2 is now sequential, so this is safe.
        if (symbol->getFunction() != nullptr)
        {
            symbol->setFunction(nullptr);
        }

        llvm::Type* retType = symbol->getReturnType()->generateLLVMType(_contextGenCode->getBackend()->getContext());
        
        std::vector<llvm::Type*> paramTypes = fillClassMethodsParamTypes(symbol);
    
        llvm::FunctionType* funcType = llvm::FunctionType::get(retType, paramTypes, false);
        
        // Name mangling for LLVM (e.g., Player_initialize)
        std::string llvmName = className;
        llvmName += '_';
        llvmName += methodName;

        llvm::Function* realFunction = llvm::Function::Create(
            funcType, 
            llvm::Function::ExternalLinkage, 
            llvmName,          
            _contextGenCode->getBackend()->getModule()
        );
        
        symbol->setFunction(realFunction);
    }
    
    // Build the vtable once after all methods have been materialized
    buildVTable(classInfo.get(), className, parentMethodList);
}

void BuilderEnvironmentRegistryFunction::fillClass()
{
    for (const auto& className : _contextGenCode->getRegistryClassGlobal()->getKeys())
    {
       fillClassMethods(className);
    }
}

void BuilderEnvironmentRegistryFunction::fill()
{   
    projectGlobalToMaterialized();
    fillClass();
}
