#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/Noeuds/Tableau/NoeudAffectationTableau.h"
#include "Compilateur/LLVM/GestionVariable.h"

void VisiteurGeneralGenCode::visiter(NoeudAffectationTableau* noeudAffectationTableau)
{
    // Évaluer l'expression de l'index
    llvm::Value* indexValue = evaluerExpression(noeudAffectationTableau->getExpressionIndex()).adresse;
  
    // Évaluer l'expression à affecter
    llvm::Value* expressionResult = evaluerExpression(noeudAffectationTableau->getExpression()).adresse;

    // Récupérer le tableau
    Symbole symbole = _contextGenCode->registreVariable->recupererVariables(noeudAffectationTableau->getToken());
    llvm::Value* valeur = symbole.adresse;

    // Affectation à un élément du tableau avec l'index
    std::vector<llvm::Value*> indices = { 
        _contextGenCode->backend->getBuilder().getInt32(0), 
        indexValue 
    };
    
    // On ne peut pas récupérer la taille du tableau à la compilation si l'index est une variable.
    // On doit donc récupérer le type du tableau directement depuis l'instruction d'allocation.
    llvm::AllocaInst* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(valeur);
    llvm::Type* arrayType = allocaInst->getAllocatedType();

    llvm::Value* ptrCase = _contextGenCode->backend->getBuilder().CreateGEP(
        arrayType, 
        valeur, 
        indices
    );

    // Cast automatique de la valeur vers le type de l'élément du tableau (ex: int -> float)
    llvm::Type* typeElement = arrayType->getArrayElementType();
    llvm::Value* valeurCastee = _contextGenCode->backend->creerAutoCast(expressionResult, typeElement);
    
    _contextGenCode->backend->getBuilder().CreateStore(valeurCastee, ptrCase);
    
    // Reset la valeur pour éviter des problèmes
    _contextGenCode->valeurTemporaire.adresse = nullptr;
}