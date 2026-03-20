#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/LLVM/GestionVariable.h"
#include <llvm-18/llvm/IR/Value.h>

void VisiteurGeneralGenCode::visiter(NoeudAffectationVariable* noeudAffectationVariable)
{
    llvm::Value* expressionResult = evaluerExpression(noeudAffectationVariable->getExpression()).getAdresse();

    AdresseurVariable adresseur(_contextGenCode);
    Symbole symbole = adresseur.recupererAdresse(noeudAffectationVariable->getToken().value);
    
    llvm::Value* variableExistante = symbole.getAdresse();
    
    llvm::Type* typeVariableLLVM = nullptr;
    if (symbole.getType() != nullptr) {
        typeVariableLLVM = symbole.getType()->genererTypeLLVM(_contextGenCode->getBackend()->getContext());
    }
    
    AffecteurVariable affecteur(_contextGenCode);
    affecteur.affecter(variableExistante, expressionResult, typeVariableLLVM);
    
    // Reset la valeur pour éviter des problèmes
    _contextGenCode->modifierValeurTemporaire(Symbole(nullptr, _contextGenCode->getValeurTemporaire().getType(), _contextGenCode->getValeurTemporaire().getTypePointeElement()));
}

