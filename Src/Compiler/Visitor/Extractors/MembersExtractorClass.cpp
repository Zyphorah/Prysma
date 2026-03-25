#include "Compiler/Visitor/Extractors/MembersExtractorClass.h"
#include "Compiler/AST/AST_Genere.h"

void MembersExtractorClass::visiter(NodeDeclarationFunction* node) { 
    methodes.push_back(node); 
}

void MembersExtractorClass::visiter(NodeDeclarationVariable* node) { 
    variables.push_back(node); 
}

void MembersExtractorClass::visiter(NodeClass* node) { 
    nomClass = node->getNomClass().value; 
}