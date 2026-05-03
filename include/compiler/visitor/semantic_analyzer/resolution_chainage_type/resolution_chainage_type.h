#include "compiler/ast/ast_genere.h"
#include "compiler/ast/registry/registry_class.h"
#include "compiler/ast/registry/types/i_type.h"
#include "compiler/visitor/visitor_base_generale.h"
#include <string>
#include <unordered_map>
#include "compiler/ast/registry/registry_function.h"

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

class ResolutionChainageType : public VisitorBaseGenerale
{

private:
    RegistryClass* _registryClass;
    std::unordered_map<std::string, IType*> _variables;
    
public: 
    explicit ResolutionChainageType(RegistryClass* registryClass) : _registryClass(registryClass){}
    ~ResolutionChainageType() override = default;

    // Delete copy and move constructors and assignment operators
    ResolutionChainageType(const ResolutionChainageType&) = delete;
    auto operator=(const ResolutionChainageType&) -> ResolutionChainageType& = delete;
    ResolutionChainageType(ResolutionChainageType&&) = delete;
    auto operator=(ResolutionChainageType&&) -> ResolutionChainageType& = delete;
    
    void visiter(NodeCallObject *nodeCallObject) override;
    void visiter(NodeDeclarationVariable *node) override;
    void visiter(NodeDeclarationObject *node) override;
    void visiter(NodeArgFunction *node) override;
    void visiter(NodeRefVariable *node) override;
};