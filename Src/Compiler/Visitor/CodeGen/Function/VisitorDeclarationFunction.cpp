#include "Compiler/LLVM/GestionFunction.h"
#include "Compiler/Visitor/CodeGen/VisitorGeneralGenCode.h"
#include "Compiler/AST/AST_Genere.h"

#include <memory>

void GeneralVisitorGenCode::visiter(NodeDeclarationFunction* nodeDeclarationFunction) 
{
    auto generator = FunctionDeclarationGenerator::create(_contextGenCode, nodeDeclarationFunction, this);
    generator->declareFunction();
}