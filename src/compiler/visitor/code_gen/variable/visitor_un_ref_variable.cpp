#include "compiler/ast/ast_genere.h"
#include "compiler/visitor/code_gen/visitor_general_gen_code.h"
#include "compiler/llvm/gestion_variable.h"

void GeneralVisitorGenCode::visiter(NodeUnRefVariable* nodeUnRefVariable)
{
    VariableLoader loader(_contextGenCode);
    _contextGenCode->setTemporaryValue(loader.loadUnref(nodeUnRefVariable->getNomVariable().value));
}