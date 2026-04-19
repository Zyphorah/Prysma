#include "compiler/visitor/code_gen/visitor_general_gen_code.h"
#include "../../../../../build/generationCode/include/compiler/ast/ast_genere_copy.txt"
#include "compiler/llvm/gestion_function.h"

void GeneralVisitorGenCode::visiter(NodeCallFunction* nodeCallFunction)
{
    auto generator = FunctionCallGenerator::create(_contextGenCode, this);
    generator->generateCallFunction(nodeCallFunction);
}