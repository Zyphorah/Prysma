#include "compiler/visitor/semantic_analyzer/resolution_chainage_type/resolution_chainage_type.h"
#include "compiler/ast/ast_genere.h"
#include "compiler/ast/nodes/interfaces/i_node.h"
#include "compiler/ast/registry/registry_class.h"
#include "compiler/ast/registry/registry_function.h"
#include "compiler/ast/registry/types/i_type.h"
#include "compiler/ast/registry/types/type_complex.h"
#include "compiler/ast/type_resolut.h"
#include "compiler/utils/prysma_cast.h"
#include <memory>
#include <stdexcept>
#include <string>

/**
 * @brief Example processing: this is a succession of calls on an object like this test.getDepth().getDepthTwo()
 *
 * @details
 * Explanation of type resolution in the call chain:
 * 1. We start by resolving the type of the receiver of the call, that is to say 'test' in the example test.getDepth().getDepthTwo().
 * It is first necessary to understand what receiver means in this context: it is the object on which the method is called, it is the starting point of the call chain, it is the link in the call chain that will allow us to resolve the type of the next call.
 *
 * 2. Then, once we have the type of the receiver, we will query the class registry to find the class corresponding to this type.
 * We use the IType* for a specific reason, that is to say that the IType* is a pointer to a structure that contains information about the type, such as its name and other useful information for type resolution.
 * It can be a complex or simple type, or an array, etc... We need this type to perform the type resolution of the next call in the chain.
 * If I am in the case of getDepthTwo() I need the return type to resolve getDepthTwo() because it is found in a different class from the base test class.
 */
 

 /*                                                                                                                                                                                                                                 
           ┌─────────────────────────────┌───────────────────────────┐          
           │NodeCallObject: getDepthTwo()│ TypeResolut (Intermediate)│          
           └────┬────────▲───────────────└───────────────────────────┘          
                │        │                                                      
                │        │                                                      
                │        │                                                      
                │        └─────────────────────────┐                            
            receiver                         provides Type_B                    
                │                                  │                            
                │                                  │                            
                │                                  │                            
           ┌────▼─────────────────────┌────────────┼──────────────┐             
           │NodeCallObject: getDepth()│ TypeResolut (Intermediate)│             
           └────┬────────▲────────────└───────────────────────────┘             
                │        │                                                      
            receiver     └────────────────────┐                                 
                │                      provides Type_A                          
                │                             │                                 
           ┌────▼───────────────┌─────────────┼─────────────┐                   
           │ NodeVariable: test │ TypeResolut (Intermediate)│                   
           └────────────────────└───────────────────────────┘                                                                                                 
 */
 
void ResolutionChainageType::visiter(NodeCallObject *nodeCallObject)
{
    // Recursively resolve the receiver (e.g., 'test' or 'test.getDepth()')
    INode* receiver = nodeCallObject->getReceiver();
    receiver->accept(this);

    // Extract the type that was just determined by the visit
    auto* receiverNode = prysma::dyn_cast<NodeCallObject>(receiver);
    IType* receiverType = nullptr;
    if ((receiverNode != nullptr) && receiverNode->getTypeResolut() != nullptr) {
        receiverType = receiverNode->getTypeResolut()->getTypeEvaluated();
    }

    if (receiverType == nullptr) {
        // Error: the previous link could not be resolved
        throw std::runtime_error("Semantic error: unable to resolve the type of the receiver in the call chain.");
    }

    auto* receiverComplexType = prysma::dyn_cast<TypeComplex>(receiverType);

    // 3. Query the registry for the current call
    // We look for 'methodName' in the class 'receiverType'
    const std::unique_ptr<Class>& classInfoPtr = _registryClass->get(receiverComplexType->getClassName());
    const auto& method = classInfoPtr->getRegistryFunctionLocal()->get(nodeCallObject->getNomMethode().value);

    if (!method) {
        // Semantic error: unknown method in this class
        throw std::runtime_error("Semantic error: method '" + std::string(nodeCallObject->getNomMethode().value) + "' not found in class '" + receiverComplexType->getClassName() + "'.");
    }

    // Decoration: inject the return type into the current node's intermediate object
    if (nodeCallObject->getTypeResolut() != nullptr) {
        if (auto* symbolLocal = prysma::dyn_cast<SymbolFunctionLocal>(method.get())) {
            nodeCallObject->getTypeResolut()->setTypeEvaluated(symbolLocal->returnType);
        } else if (auto* symbolGlobal = prysma::dyn_cast<SymbolFunctionGlobal>(method.get())) {
            nodeCallObject->getTypeResolut()->setTypeEvaluated(symbolGlobal->returnType);
        }
    }
}