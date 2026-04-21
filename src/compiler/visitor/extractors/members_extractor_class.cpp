#include "compiler/visitor/extractors/members_extractor_class.h"
#include "compiler/ast/ast_genere.h"

void MembersExtractorClass::visiter(NodeDeclarationFunction* node) { 
    methods.push_back(node); 
}

void MembersExtractorClass::visiter(NodeDeclarationVariable* node) { 
    variables.push_back(node); 
}

void MembersExtractorClass::visiter(NodeClass* node) { 
    className = node->getNomClass().value; 
}