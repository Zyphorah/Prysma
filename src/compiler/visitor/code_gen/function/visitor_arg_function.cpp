#include "compiler/ast/registry/stack/registry_variable.h"
#include "compiler/visitor/code_gen/visitor_general_gen_code.h"
#include "compiler/macros/prysma_maybe_unused.h"
#include "compiler/ast/ast_genere.h"

void GeneralVisitorGenCode::visiter(PRYSMA_MAYBE_UNUSED NodeArgFunction* nodeArgFunction) 
{
    _contextGenCode->setTemporaryValue(Symbol(nullptr, _contextGenCode->getTemporaryValue().getType(), _contextGenCode->getTemporaryValue().getPointedElementType()));
}