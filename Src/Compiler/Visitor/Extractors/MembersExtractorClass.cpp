#include "Compiler/Visitor/Extractors/MembersExtractorClass.h"
#include "Compiler/AST/AST_Genere.h"

void MembersExtractorClass::visiter(NodeDeclarationFunction* node) { 
    methods.push_back(node); 
}

void MembersExtractorClass::visiter(NodeDeclarationVariable* node) { 
    variables.push_back(node); 
}

void MembersExtractorClass::visiter(NodeClass* node) { 
    className = node->getNomClass().value; 
}