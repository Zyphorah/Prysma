#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Registry/Stack/RegistryVariable.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Visitor/CodeGen/VisitorGeneralGenCode.h"
#include <llvm/IR/Value.h>

void GeneralVisitorGenCode::visiter(NodeRefVariable* nodeRefVariable) 
{
    Token variableToken;
    variableToken.value = nodeRefVariable->getNomVariable();
    Symbol symbol = _contextGenCode->getRegistryVariable()->getVariable(variableToken);
    llvm::Value* variable = symbol.getAddress();
    _contextGenCode->setTemporaryValue(Symbol(variable, _contextGenCode->getTemporaryValue().getType(), _contextGenCode->getTemporaryValue().getPointedElementType()));
    _contextGenCode->setTemporaryValue(Symbol(_contextGenCode->getTemporaryValue().getAddress(), symbol.getType(), _contextGenCode->getTemporaryValue().getPointedElementType()));
}
