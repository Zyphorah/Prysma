#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/AST/Registre/Types/TypeSimple.h"
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LLVMContext.h>
#include "Compilateur/Visiteur/CodeGen/Helper/ErrorHelper.h"

void VisiteurGeneralGenCode::visiter(NoeudNegation* noeud)
{
    // Vérification de sécurité
    if (noeud == nullptr || noeud->getOperande() == nullptr) {
        ErrorHelper::erreurCompilation("NoeudNegation ou opérande invalide");
    }
    
    // Évaluer l'opérande (doit être un booléen)
    noeud->getOperande()->accept(this);
    llvm::Value* valOperande = _contextGenCode->getValeurTemporaire().getAdresse();

    auto& builder = _contextGenCode->getBackend()->getBuilder();
    llvm::LLVMContext& context = _contextGenCode->getBackend()->getContext();
    
    // S'assurer que l'opérande est du type i1 (bool)
    if (!valOperande->getType()->isIntegerTy(1)) {
        valOperande = builder.CreateTrunc(valOperande, llvm::Type::getInt1Ty(context), "tobool");
    }
    
    // Créer la négation logique avec NOT
    llvm::Value* resultat = builder.CreateNot(valOperande, "not");

    _contextGenCode->modifierValeurTemporaire(Symbole(resultat, _contextGenCode->getValeurTemporaire().getType(), _contextGenCode->getValeurTemporaire().getTypePointeElement()));
    _contextGenCode->modifierValeurTemporaire(Symbole(_contextGenCode->getValeurTemporaire().getAdresse(), new (_contextGenCode->getArena()->Allocate<TypeSimple>()) TypeSimple(llvm::Type::getInt1Ty(context)), _contextGenCode->getValeurTemporaire().getTypePointeElement()));
}
