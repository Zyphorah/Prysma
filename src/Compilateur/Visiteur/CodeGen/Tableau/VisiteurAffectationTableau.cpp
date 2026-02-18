#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/Noeuds/Tableau/NoeudAffectationTableau.h"
#include "Compilateur/LLVM/GestionVariable.h"

void VisiteurGeneralGenCode::visiter(NoeudAffectationTableau* noeudAffectationTableau)
{
    // Évaluer l'expression de l'index
    llvm::Value* indexValue = evaluerExpression(noeudAffectationTableau->getExpressionIndex());

    // Évaluer l'expression à affecter
    llvm::Value* expressionResult = evaluerExpression(noeudAffectationTableau->getExpression());

    // Récupérer le tableau
    llvm::Value* valeur = _contextGenCode->registreVariable->recupererVariables(noeudAffectationTableau->getToken());

    // Affectation à un élément du tableau avec l'index
    std::vector<llvm::Value*> indices = { 
        _contextGenCode->backend->getBuilder().getInt32(0), 
        indexValue 
    };
    
    llvm::Value* ptrCase = _contextGenCode->backend->getBuilder().CreateGEP(
        llvm::ArrayType::get(_contextGenCode->backend->getBuilder().getInt32Ty(), 4), 
        valeur, 
        indices
    );
    
    _contextGenCode->backend->getBuilder().CreateStore(expressionResult, ptrCase);
    
    // Reset la valeur pour éviter des problèmes
    _contextGenCode->valeurTemporaire = nullptr;
}