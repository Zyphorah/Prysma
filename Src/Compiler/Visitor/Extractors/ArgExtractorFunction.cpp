#include "Compiler/Visitor/Extractors/ArgExtractorFunction.h"
#include "Compiler/AST/AST_Genere.h"

void ArgExtractorFunction::visiter(NodeArgFunction* node) {
    arg = node;
}