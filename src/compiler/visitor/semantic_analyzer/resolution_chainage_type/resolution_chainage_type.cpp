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