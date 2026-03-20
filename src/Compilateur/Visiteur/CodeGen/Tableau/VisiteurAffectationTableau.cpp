#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/AST_Genere.h"
#include <llvm-18/llvm/IR/Value.h>
#include <llvm/Support/Casting.h>
#include <vector>

void VisiteurGeneralGenCode::visiter(NoeudAffectationTableau* noeudAffectationTableau)
{
    // Évaluer l'expression de l'index
    llvm::Value* indexValue = evaluerExpression(noeudAffectationTableau->getExpressionIndex()).getAdresse();
  
    // Évaluer l'expression à affecter
    llvm::Value* expressionResult = evaluerExpression(noeudAffectationTableau->getExpression()).getAdresse();

    // Récupérer le tableau
    Symbole symbole = _contextGenCode->getRegistreVariable()->recupererVariables(noeudAffectationTableau->getToken());
    llvm::Value* valeur = symbole.getAdresse();

    // Affectation à un élément du tableau avec l'index
    std::vector<llvm::Value*> indices = { 
        _contextGenCode->getBackend()->getBuilder().getInt32(0), 
        indexValue 
    };
    
    // On ne peut pas récupérer la taille du tableau à la compilation si l'index est une variable.
    // On doit donc récupérer le type du tableau directement depuis l'instruction d'allocation.
    auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(valeur);
    llvm::Type* arrayType = allocaInst->getAllocatedType();

    llvm::Value* ptrCase = _contextGenCode->getBackend()->getBuilder().CreateGEP(
        arrayType, 
        valeur, 
        indices
    );

    // Cast automatique de la valeur vers le type de l'élément du tableau (ex: int -> float)
    llvm::Type* typeElement = arrayType->getArrayElementType();
    llvm::Value* valeurCastee = _contextGenCode->getBackend()->creerAutoCast(expressionResult, typeElement);
    
    _contextGenCode->getBackend()->getBuilder().CreateStore(valeurCastee, ptrCase);
    
    // Reset la valeur pour éviter des problèmes
    _contextGenCode->modifierValeurTemporaire(Symbole(nullptr, _contextGenCode->getValeurTemporaire().getType(), _contextGenCode->getValeurTemporaire().getTypePointeElement()));
}