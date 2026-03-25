#include "Compiler/Visitor/CodeGen/VisitorGeneralGenCode.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/LLVM/GestionFunction.h"

void GeneralVisitorGenCode::visiter(NodeCallFunction* nodeCallFunction)
{
    auto generator = GenerateurCallFunction::creer(_contextGenCode, this);
    generator->generatedrCallFunction(nodeCallFunction);
}