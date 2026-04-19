#include "compiler/llvm/gestion_function.h"
#include "compiler/visitor/code_gen/visitor_general_gen_code.h"
#include "../../../../../build/generationCode/include/compiler/ast/ast_genere_copy.txt"

#include <memory>

void GeneralVisitorGenCode::visiter(NodeDeclarationFunction* nodeDeclarationFunction) 
{
    auto generator = FunctionDeclarationGenerator::create(_contextGenCode, nodeDeclarationFunction, this);
    generator->declareFunction();
}