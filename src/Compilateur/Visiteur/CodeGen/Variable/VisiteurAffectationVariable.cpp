#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/LLVM/GestionVariable.h"

void VisiteurGeneralGenCode::visiter(NoeudAffectationVariable* noeudAffectationVariable)
{
    GestionVariable gestionVariable(_contextGenCode);

    llvm::Value* expressionResult = evaluerExpression(noeudAffectationVariable->getExpression()).adresse;

    Symbole symbole = gestionVariable.recupererAdresseVariable(noeudAffectationVariable->getToken().value);
    llvm::Value* variableExistante = symbole.adresse;
    
    llvm::Type* typeVariableLLVM = nullptr;
    if (symbole.type != nullptr) {
        typeVariableLLVM = symbole.type->genererTypeLLVM(_contextGenCode->backend->getContext());
    }
    
    gestionVariable.affecterVariable(variableExistante, expressionResult, typeVariableLLVM);
    
    // Reset la valeur pour éviter des problèmes
    _contextGenCode->valeurTemporaire.adresse = nullptr;
}

