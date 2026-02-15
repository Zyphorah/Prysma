#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/Noeuds/Operande/NoeudNegation.h"
#include <llvm/IR/Instructions.h>

void VisiteurGeneralGenCode::visiter(NoeudNegation* noeud)
{
    // Évaluer l'opérande (doit être un booléen)
    noeud->getOperande()->accept(this);
    llvm::Value* valOperande = _contextGenCode->valeurTemporaire;

    auto& builder = _contextGenCode->backend->getBuilder();
    
    // Créer la négation logique (XOR avec 1)
    llvm::Value* one = llvm::ConstantInt::get(llvm::Type::getInt1Ty(_contextGenCode->backend->getContext()), 1);
    llvm::Value* resultat = builder.CreateXor(valOperande, one, "not");

    _contextGenCode->valeurTemporaire = resultat;
}
