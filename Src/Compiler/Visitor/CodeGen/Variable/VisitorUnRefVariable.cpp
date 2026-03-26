#include "Compiler/AST/AST_Genere.h"
#include "Compiler/Visitor/CodeGen/VisitorGeneralGenCode.h"
#include "Compiler/LLVM/GestionVariable.h"

void GeneralVisitorGenCode::visiter(NodeUnRefVariable* nodeUnRefVariable) 
{
    VariableLoader loader(_contextGenCode);
    _contextGenCode->setTemporaryValue(loader.loadUnref(nodeUnRefVariable->getNomVariable()));
}