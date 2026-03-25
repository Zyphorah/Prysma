#include "Compiler/AST/AST_Genere.h"
#include "Compiler/Visitor/CodeGen/VisitorGeneralGenCode.h"
#include "Compiler/LLVM/GestionVariable.h"

void GeneralVisitorGenCode::visiter(NodeUnRefVariable* nodeUnRefVariable) 
{
    LoadurVariable loadur(_contextGenCode);
    
    _contextGenCode->modifierValeurTemporaire(loadur.loadrUnref(nodeUnRefVariable->getNomVariable()));
}