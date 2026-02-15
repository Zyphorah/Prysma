#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/Noeuds/Operande/NoeudNegation.h"
#include <llvm/IR/Instructions.h>

void VisiteurGeneralGenCode::visiter(NoeudNegation* noeud)
{
    // Évaluer l'opérande (doit être un booléen)
    noeud->getOperande()->accept(this);
    llvm::Value* valOperande = _contextGenCode->valeurTemporaire;

    auto& builder = _contextGenCode->backend->getBuilder();
    llvm::LLVMContext& context = _contextGenCode->backend->getContext();
    
    // S'assurer que l'opérande est du type i1 (bool)
    if (!valOperande->getType()->isIntegerTy(1)) {
        valOperande = builder.CreateTrunc(valOperande, llvm::Type::getInt1Ty(context), "tobool");
    }
    
    // Créer la négation logique avec NOT
    llvm::Value* resultat = builder.CreateNot(valOperande, "not");

    _contextGenCode->valeurTemporaire = resultat;
}
