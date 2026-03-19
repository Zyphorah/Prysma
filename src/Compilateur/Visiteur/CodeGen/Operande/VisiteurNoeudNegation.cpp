#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/AST/Registre/Types/TypeSimple.h"
#include <llvm/IR/Instructions.h>
#include "Compilateur/Visiteur/CodeGen/Helper/ErrorHelper.h"

void VisiteurGeneralGenCode::visiter(NoeudNegation* noeud)
{
    // Vérification de sécurité
    if (noeud == nullptr || noeud->getOperande() == nullptr) {
        ErrorHelper::erreurCompilation("NoeudNegation ou opérande invalide");
    }
    
    // Évaluer l'opérande (doit être un booléen)
    noeud->getOperande()->accept(this);
    llvm::Value* valOperande = _contextGenCode->valeurTemporaire.adresse;

    auto& builder = _contextGenCode->backend->getBuilder();
    llvm::LLVMContext& context = _contextGenCode->backend->getContext();
    
    // S'assurer que l'opérande est du type i1 (bool)
    if (!valOperande->getType()->isIntegerTy(1)) {
        valOperande = builder.CreateTrunc(valOperande, llvm::Type::getInt1Ty(context), "tobool");
    }
    
    // Créer la négation logique avec NOT
    llvm::Value* resultat = builder.CreateNot(valOperande, "not");

    _contextGenCode->valeurTemporaire.adresse = resultat;
    _contextGenCode->valeurTemporaire.type = new (_contextGenCode->arena->Allocate<TypeSimple>()) TypeSimple(llvm::Type::getInt1Ty(context));
}
