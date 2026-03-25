#include "Compiler/AST/Registry/Stack/RegistryVariable.h"
#include "Compiler/Visitor/CodeGen/VisitorGeneralGenCode.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/LLVM/GestionVariable.h"
#include <llvm-18/llvm/IR/Value.h>

void GeneralVisitorGenCode::visiter(NodeAssignmentVariable* nodeAssignmentVariable)
{
    llvm::Value* expressionResult = evaluerExpression(nodeAssignmentVariable->getExpression()).getAdresse();

    AdresseurVariable adresseur(_contextGenCode);
    Symbole symbole = adresseur.recupererAdresse(nodeAssignmentVariable->getToken().value);
    
    llvm::Value* variableExistante = symbole.getAdresse();
    
    llvm::Type* typeVariableLLVM = nullptr;
    if (symbole.getType() != nullptr) {
        typeVariableLLVM = symbole.getType()->generatedrTypeLLVM(_contextGenCode->getBackend()->getContext());
    }
    
    AffecteurVariable affecteur(_contextGenCode);
    affecteur.affecter(variableExistante, expressionResult, typeVariableLLVM);
    
    // Reset la valeur pour éviter des problèmes
    _contextGenCode->modifierValeurTemporaire(Symbole(nullptr, _contextGenCode->getValeurTemporaire().getType(), _contextGenCode->getValeurTemporaire().getTypePointeElement()));
}

