#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/Noeuds/Variable/NoeudAffectationVariable.h"
#include "Compilateur/LLVM/GestionVariable.h"

void VisiteurGeneralGenCode::visiter(NoeudAffectationVariable* noeudAffectationVariable)
{
    GestionVariable gestionVariable(_contextGenCode);

    llvm::Value* expressionResult = evaluerExpression(noeudAffectationVariable->_expression);

    llvm::Value* valeur = _contextGenCode->registreVariable->recupererVariables(noeudAffectationVariable->_token);
    llvm::AllocaInst* variableExistante = llvm::dyn_cast<llvm::AllocaInst>(valeur);
    
    gestionVariable.affecterVariable(variableExistante, expressionResult);
    
    // Reset la valeur pour éviter des problèmes
    _contextGenCode->valeurTemporaire = nullptr;
}

