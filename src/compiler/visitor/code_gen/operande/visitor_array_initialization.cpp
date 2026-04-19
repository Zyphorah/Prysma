#include "compiler/ast/registry/stack/registry_variable.h"
#include "compiler/visitor/code_gen/visitor_general_gen_code.h"
#include "compiler/macros/prysma_maybe_unused.h"
#include "../../../../../build/generationCode/include/compiler/ast/ast_genere_copy.txt"

void GeneralVisitorGenCode::visiter(NodeArrayInitialization* nodeInitialization PRYSMA_MAYBE_UNUSED)
{
    // Cannot really initialize an array as a "temporary constant"
    // because arrays need to be allocated. This visitor will be called during
    // the evaluation of an expression containing [1, 2, 3, ...].
    // For now, return nullptr - the real handling must be done
    // in VisitorDeclarationVariable which knows the array type.
    
    _contextGenCode->setTemporaryValue(Symbol(nullptr, _contextGenCode->getTemporaryValue().getType(), _contextGenCode->getTemporaryValue().getPointedElementType()));
    _contextGenCode->setTemporaryValue(Symbol(_contextGenCode->getTemporaryValue().getAddress(), nullptr, _contextGenCode->getTemporaryValue().getPointedElementType()));
}
