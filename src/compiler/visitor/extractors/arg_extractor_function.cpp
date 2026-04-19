#include "compiler/visitor/extractors/arg_extractor_function.h"
#include "../../../../build/generationCode/include/compiler/ast/ast_genere_copy.txt"

void ArgExtractorFunction::visiter(NodeArgFunction* node) {
    arg = node;
}