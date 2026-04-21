#include "compiler/visitor/extractors/arg_extractor_function.h"

void ArgExtractorFunction::visiter(NodeArgFunction* node) {
    arg = node;
}