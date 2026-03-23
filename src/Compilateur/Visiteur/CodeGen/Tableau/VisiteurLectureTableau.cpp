#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include "Compilateur/AST/Registre/Types/IType.h"
#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/AST/Registre/Types/TypeSimple.h"
#include "Compilateur/AST/Registre/Types/TypeTableau.h"
#include "Compilateur/Utils/PrysmaCast.h"
#include "Compilateur/Visiteur/CodeGen/Helper/ErrorHelper.h"
#include <llvm-18/llvm/IR/Instructions.h>
#include <llvm-18/llvm/Support/Casting.h>

void VisiteurGeneralGenCode::visiter(NoeudLectureTableau* noeudLectureTableau)
{
    // Récupération de l'adresse de base du tableau 
    Symbole symbole = _contextGenCode->getRegistreVariable()->recupererVariables(noeudLectureTableau->getNomTableau());
    llvm::Value* adresseTableau = symbole.getAdresse();

    // Faire une allocaInstance pour récupérer le type, on dyn cast le type llvm::Value 
    auto* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(adresseTableau);
    allocaInst = ErrorHelper::verifierNonNull(allocaInst, "L'adresse du tableau n'est pas une instruction d'allocation");
    llvm::Type* typeTableau = allocaInst->getAllocatedType();
    
    // Calculer l'index à partir de l'équation 
    noeudLectureTableau->getIndexEquation()->accept(this);
    llvm::Value* indexValue = _contextGenCode->getValeurTemporaire().getAdresse();
    
    llvm::Value* adresseElement = nullptr;
    llvm::Type* typeElement = nullptr;

    if (typeTableau->isArrayTy()) {
        // calculer le décalage avec GEP 
        adresseElement = _contextGenCode->getBackend()->getBuilder().CreateGEP(typeTableau, adresseTableau, 
            { 
                _contextGenCode->getBackend()->getBuilder().getInt32(0), 
                indexValue 
            }, noeudLectureTableau->getNomTableau().value);
        typeElement = typeTableau->getArrayElementType();
    } else {
        // C'est un pointeur vers un tableau (référence), on doit charger le pointeur
        llvm::Value* loadedPtr = _contextGenCode->getBackend()->getBuilder().CreateLoad(typeTableau, adresseTableau);
        
        IType* astType = symbole.getType();
        auto* typeTableauDecla = prysma::dyn_cast<TypeTableau>(astType);
        if (typeTableauDecla != nullptr) {
            typeElement = typeTableauDecla->getTypeEnfant()->genererTypeLLVM(_contextGenCode->getBackend()->getContext());
        } else {
            typeElement = symbole.getTypePointeElement();
        }
        
        adresseElement = _contextGenCode->getBackend()->getBuilder().CreateGEP(
            typeElement, 
            loadedPtr, 
            indexValue,
            noeudLectureTableau->getNomTableau().value
        );
    }
        
    // Lire la valeur de l'adresse de l'élément
    llvm::Value* valeurElement = _contextGenCode->getBackend()->getBuilder().CreateLoad(typeElement, adresseElement, noeudLectureTableau->getNomTableau().value);

    _contextGenCode->modifierValeurTemporaire(Symbole(valeurElement, _contextGenCode->getValeurTemporaire().getType(), _contextGenCode->getValeurTemporaire().getTypePointeElement()));
    _contextGenCode->modifierValeurTemporaire(Symbole(_contextGenCode->getValeurTemporaire().getAdresse(), new (_contextGenCode->getArena()->Allocate<TypeSimple>()) TypeSimple(typeElement), _contextGenCode->getValeurTemporaire().getTypePointeElement()));
} 

