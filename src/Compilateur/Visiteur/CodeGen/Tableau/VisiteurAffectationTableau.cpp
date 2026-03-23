#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include "Compilateur/AST/Registre/Types/IType.h"
#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/AST/Registre/Types/TypeTableau.h"
#include "Compilateur/Utils/PrysmaCast.h"
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

    llvm::Value* ptrCase = nullptr;
    llvm::Type* typeElement = nullptr;

    if (arrayType->isArrayTy()) {
        ptrCase = _contextGenCode->getBackend()->getBuilder().CreateGEP(
            arrayType, 
            valeur, 
            indices
        );
        typeElement = arrayType->getArrayElementType();
    } else {
        // C'est un pointeur vers un tableau (référence), on doit charger le pointeur
        llvm::Value* loadedPtr = _contextGenCode->getBackend()->getBuilder().CreateLoad(arrayType, valeur);
        
        IType* astType = symbole.getType();
        auto* typeTableauDecla = prysma::dyn_cast<TypeTableau>(astType);
        if (typeTableauDecla != nullptr) {
            typeElement = typeTableauDecla->getTypeEnfant()->genererTypeLLVM(_contextGenCode->getBackend()->getContext());
        } else {
            typeElement = symbole.getTypePointeElement();
        }
        
        ptrCase = _contextGenCode->getBackend()->getBuilder().CreateGEP(
            typeElement, 
            loadedPtr, 
            indexValue
        );
    }

    llvm::Value* valeurCastee = _contextGenCode->getBackend()->creerAutoCast(expressionResult, typeElement);
    
    _contextGenCode->getBackend()->getBuilder().CreateStore(valeurCastee, ptrCase);
    
    // Reset la valeur pour éviter des problèmes
    _contextGenCode->modifierValeurTemporaire(Symbole(nullptr, _contextGenCode->getValeurTemporaire().getType(), _contextGenCode->getValeurTemporaire().getTypePointeElement()));
}