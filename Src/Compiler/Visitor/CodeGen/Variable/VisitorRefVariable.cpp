#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Registry/Stack/RegistryVariable.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Visitor/CodeGen/VisitorGeneralGenCode.h"
#include <llvm/IR/Value.h>

void GeneralVisitorGenCode::visiter(NodeRefVariable* nodeRefVariable) 
{
    Token variableToken;
    variableToken.value = nodeRefVariable->getNomVariable();
    Symbole symbole = _contextGenCode->getRegistryVariable()->recupererVariables(variableToken);
    llvm::Value* variable = symbole.getAdresse();
    _contextGenCode->modifierValeurTemporaire(Symbole(variable, _contextGenCode->getValeurTemporaire().getType(), _contextGenCode->getValeurTemporaire().getTypePointeElement()));
    _contextGenCode->modifierValeurTemporaire(Symbole(_contextGenCode->getValeurTemporaire().getAdresse(), symbole.getType(), _contextGenCode->getValeurTemporaire().getTypePointeElement()));
}
