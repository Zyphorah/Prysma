#include "compiler/visitor/extractors/arg_extractor_function.h"
#include "compiler/ast/registry/context_gen_code.h"

ArgExtractorFunction::ArgExtractorFunction(ContextGenCode* contextGenCode)
    : _contextGenCode(contextGenCode)
{} // inutile pour l'instant mais c'est pour la cohérence et l'extensibilité

ArgExtractorFunction::~ArgExtractorFunction()
= default;

void ArgExtractorFunction::visiter(NodeArgFunction* nodeArgFunction) {
    arg = nodeArgFunction;
}