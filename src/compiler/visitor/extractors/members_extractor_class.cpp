#include "compiler/visitor/extractors/members_extractor_class.h"
#include "compiler/ast/ast_genere.h"
#include "compiler/ast/registry/context_gen_code.h"
#include "compiler/ast/registry/node_component_registry.h"

MembersExtractorClass::MembersExtractorClass(ContextGenCode* contextGenCode)
    : _contextGenCode(contextGenCode) {}

MembersExtractorClass::~MembersExtractorClass()
= default;

void MembersExtractorClass::visiter(NodeDeclarationFunction* nodeClass) { 
    methods.push_back(nodeClass); 
}

void MembersExtractorClass::visiter(NodeDeclarationVariable* nodeClass) { 
    variables.push_back(nodeClass); 
}

void MembersExtractorClass::visiter(NodeClass* nodeClass) { 
    auto& nodeClassData = _contextGenCode->getNodeComponentRegistry()
        ->get<NodeClassComponents>(nodeClass->getNodeId());

    className = nodeClassData.getName().value; 
}