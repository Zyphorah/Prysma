#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include <llvm/IR/Value.h>

void VisiteurGeneralGenCode::visiter(NoeudRefVariable* noeudRefVariable) 
{
    Token variableToken;
    variableToken.value = noeudRefVariable->getNomVariable();
    Symbole symbole = _contextGenCode->getRegistreVariable()->recupererVariables(variableToken);
    llvm::Value* variable = symbole.getAdresse();
    _contextGenCode->modifierValeurTemporaire(Symbole(variable, _contextGenCode->getValeurTemporaire().getType(), _contextGenCode->getValeurTemporaire().getTypePointeElement()));
    _contextGenCode->modifierValeurTemporaire(Symbole(_contextGenCode->getValeurTemporaire().getAdresse(), symbole.getType(), _contextGenCode->getValeurTemporaire().getTypePointeElement()));
}
