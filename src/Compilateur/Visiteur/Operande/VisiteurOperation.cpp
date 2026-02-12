#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/Noeuds/Equation/NoeudOperation.h"
#include <llvm/IR/Type.h>

void VisiteurGeneralGenCode::visiter(NoeudOperation* noeud)
{

    noeud->gauche->accept(this);
    llvm::Value* valGauche = _contextGenCode->valeurTemporaire;


    noeud->droite->accept(this);
    llvm::Value* valDroite = _contextGenCode->valeurTemporaire;


    llvm::Type* floatTy = llvm::Type::getFloatTy(_contextGenCode->backend->getContext());
    valGauche = _contextGenCode->backend->creerAutoCast(valGauche, floatTy);
    valDroite = _contextGenCode->backend->creerAutoCast(valDroite, floatTy);

    llvm::Value* resultat = nullptr;
    auto& builder = _contextGenCode->backend->getBuilder();

    switch (noeud->typeOperation) {
        case TOKEN_PLUS:
            resultat = builder.CreateFAdd(valGauche, valDroite, "addtmp");
            break;
        case TOKEN_MOINS:
            resultat = builder.CreateFSub(valGauche, valDroite, "subtmp");
            break;
        case TOKEN_ETOILE:
            resultat = builder.CreateFMul(valGauche, valDroite, "multmp");
            break;
        case TOKEN_SLASH:
            resultat = builder.CreateFDiv(valGauche, valDroite, "divtmp");
            break;
        default:
            throw std::runtime_error("Erreur: type d'opération non reconnu dans VisiteurOperation");
    }
    
    if (resultat == nullptr) {
        throw std::runtime_error("Erreur: le résultat de l'opération est nullptr");
    }
    
    _contextGenCode->valeurTemporaire = resultat;
}
