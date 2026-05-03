#include "compiler/visitor/semantic_analyzer/resolution_chainage_type/resolution_chainage_type.h"
#include "compiler/ast/ast_genere.h"
#include "compiler/ast/nodes/interfaces/i_node.h"
#include "compiler/ast/registry/registry_class.h"
#include "compiler/ast/registry/registry_function.h"
#include "compiler/ast/registry/types/i_type.h"
#include "compiler/ast/registry/types/type_complex.h"
#include "compiler/ast/type_resolut.h"
#include "compiler/utils/prysma_cast.h"
#include "compiler/visitor/visitor_base_generale.h"
#include <memory>
#include <stdexcept>
#include <string>

 
void ResolutionChainageType::visiter(NodeDeclarationVariable *node) {
    if (node->getType() != nullptr) {
        _variables[std::string(node->getNom().value)] = node->getType();
    }
    VisitorBaseGenerale::visiter(node);
}

void ResolutionChainageType::visiter(NodeDeclarationObject *node) {
    if (node->getTypeObject() != nullptr) {
        _variables[std::string(node->getNomObject().value)] = node->getTypeObject();
    }
    VisitorBaseGenerale::visiter(node);
}

void ResolutionChainageType::visiter(NodeArgFunction *node) {
    if (node->getType() != nullptr) {
        _variables[std::string(node->getNom().value)] = node->getType();
    }
    VisitorBaseGenerale::visiter(node);
}

void ResolutionChainageType::visiter(NodeRefVariable *node) {
    std::string varName = std::string(node->getNomVariable().value);
    
    auto it = _variables.find(varName);
    if (it != _variables.end()) {
        if (node->getTypeResolut() != nullptr) {
            node->getTypeResolut()->setTypeEvaluated(it->second);
        }
    }
}

void ResolutionChainageType::visiter(NodeCallObject *nodeCallObject)
{
    // Recursive resolution of the receiver
    INode* receiver = nodeCallObject->getReceiver();
    receiver->accept(this);
    IType* receiverType = nullptr;

    // We try to retrieve the TypeResolut in a generic way
    if (auto* res = prysma::dyn_cast<NodeCallObject>(receiver)) {
        receiverType = res->getTypeResolut()->getTypeEvaluated();
    } else if (auto* res = prysma::dyn_cast<NodeRefVariable>(receiver)) {
        receiverType = res->getTypeResolut()->getTypeEvaluated();
    }

    if (receiverType == nullptr) {
        throw std::runtime_error("Semantic error: unable to resolve type for node " + std::to_string(static_cast<int>(receiver->getGeneratedType())));
    }

    auto* receiverComplexType = prysma::dyn_cast<TypeComplex>(receiverType);
    if (receiverComplexType == nullptr) {
        throw std::runtime_error("Semantic error: receiver is not a complex type (class).");
    }

    // Method lookup
    const std::unique_ptr<Class>& classInfoPtr = _registryClass->get(receiverComplexType->getClassName());
    if (!classInfoPtr) {
        throw std::runtime_error("Semantic error: class '" + receiverComplexType->getClassName() + "' not found.");
    }

    const auto& method = classInfoPtr->getRegistryFunctionLocal()->get(nodeCallObject->getNomMethode().value);
    if (!method) {
        throw std::runtime_error("Semantic error: method '" + std::string(nodeCallObject->getNomMethode().value) + "' not found in class '" + receiverComplexType->getClassName() + "'.");
    }

    // Decoration
    if (nodeCallObject->getTypeResolut() != nullptr) {
        if (auto* symbolLocal = prysma::dyn_cast<SymbolFunctionLocal>(method.get())) {
            nodeCallObject->getTypeResolut()->setTypeEvaluated(symbolLocal->returnType);
        } else if (auto* symbolGlobal = prysma::dyn_cast<SymbolFunctionGlobal>(method.get())) {
            nodeCallObject->getTypeResolut()->setTypeEvaluated(symbolGlobal->returnType);
        }
    }
}