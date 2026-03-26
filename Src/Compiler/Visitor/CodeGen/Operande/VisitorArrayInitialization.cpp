#include "Compiler/AST/Registry/Stack/RegistryVariable.h"
#include "Compiler/Visitor/CodeGen/VisitorGeneralGenCode.h"
#include "Compiler/AST/AST_Genere.h"

void GeneralVisitorGenCode::visiter(NodeArrayInitialization* nodeInitialization [[maybe_unused]])
{
    // Cannot really initialize an array as a "temporary constant"
    // because arrays need to be allocated. This visitor will be called during
    // the evaluation of an expression containing [1, 2, 3, ...].
    // For now, return nullptr - the real handling must be done
    // in VisitorDeclarationVariable which knows the array type.
    
    _contextGenCode->setTemporaryValue(Symbol(nullptr, _contextGenCode->getTemporaryValue().getType(), _contextGenCode->getTemporaryValue().getPointedElementType()));
    _contextGenCode->setTemporaryValue(Symbol(_contextGenCode->getTemporaryValue().getAddress(), nullptr, _contextGenCode->getTemporaryValue().getPointedElementType()));
}
