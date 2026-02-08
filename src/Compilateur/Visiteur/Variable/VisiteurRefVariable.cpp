#include "Compilateur/AST/Noeuds/Variable/NoeudRefVariable.h"
#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"

void VisiteurGeneralGenCode::visiter(NoeudRefVariable* noeudRefVariable) 
{
    Token variableToken;
    variableToken.value = noeudRefVariable->_nomVariable;
    llvm::Value* variable = _contextGenCode->registreVariable->recupererVariables(variableToken);
    _contextGenCode->valeurTemporaire = variable;
}
