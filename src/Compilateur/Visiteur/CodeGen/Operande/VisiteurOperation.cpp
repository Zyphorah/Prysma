#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/AST/Registre/Types/TypeSimple.h"
#include <llvm/IR/Type.h>

#include "Compilateur/Visiteur/CodeGen/Helper/ErrorHelper.h"

void VisiteurGeneralGenCode::visiter(NoeudOperation* noeud)
{
    // On évalue la gauche et on SAUVEGARDE le Symbole complet
    noeud->getGauche()->accept(this);
    Symbole symGauche = _contextGenCode->valeurTemporaire;

    // On évalue la droite (le valeurTemporaire de gauche n'est plus écrasé)
    noeud->getDroite()->accept(this);
    Symbole symDroite = _contextGenCode->valeurTemporaire;

    llvm::Value* valGauche = symGauche.adresse;
    llvm::Value* valDroite = symDroite.adresse;

    auto& builder = _contextGenCode->backend->getBuilder();
    llvm::Value* resultat = nullptr;
    llvm::Type* typeResultat = nullptr;

    // C'est Prysma qui décide du type, plus LLVM
    bool estFlottant = (symGauche.type != nullptr && symGauche.type->estFlottant()) || 
                       (symDroite.type != nullptr && symDroite.type->estFlottant());

    if (estFlottant) {
        llvm::Type* floatTy = llvm::Type::getFloatTy(_contextGenCode->backend->getContext());
        valGauche = _contextGenCode->backend->creerAutoCast(valGauche, floatTy);
        valDroite = _contextGenCode->backend->creerAutoCast(valDroite, floatTy);
        typeResultat = floatTy;

        switch (noeud->getToken().type) {
            case TOKEN_PLUS:  resultat = builder.CreateFAdd(valGauche, valDroite, "fadd"); break;
            case TOKEN_MOINS: resultat = builder.CreateFSub(valGauche, valDroite, "fsub"); break;
            case TOKEN_ETOILE:resultat = builder.CreateFMul(valGauche, valDroite, "fmul"); break;
            case TOKEN_SLASH: resultat = builder.CreateFDiv(valGauche, valDroite, "fdiv"); break;
            case TOKEN_MODULO: resultat = builder.CreateFRem(valGauche, valDroite, "frem"); break;
            // Pour les comparaisons float, ça renvoie un i1 (bool)
            case TOKEN_PLUS_PETIT:      resultat = builder.CreateFCmpOLT(valGauche, valDroite, "fcmp_lt"); typeResultat = llvm::Type::getInt1Ty(builder.getContext()); break;
            case TOKEN_PLUS_GRAND:      resultat = builder.CreateFCmpOGT(valGauche, valDroite, "fcmp_gt"); typeResultat = llvm::Type::getInt1Ty(builder.getContext()); break;
            case TOKEN_PLUS_PETIT_EGAL: resultat = builder.CreateFCmpOLE(valGauche, valDroite, "fcmp_le"); typeResultat = llvm::Type::getInt1Ty(builder.getContext()); break;
            case TOKEN_PLUS_GRAND_EGAL: resultat = builder.CreateFCmpOGE(valGauche, valDroite, "fcmp_ge"); typeResultat = llvm::Type::getInt1Ty(builder.getContext()); break;
            case TOKEN_EGAL_EGAL:       resultat = builder.CreateFCmpOEQ(valGauche, valDroite, "fcmp_eq"); typeResultat = llvm::Type::getInt1Ty(builder.getContext()); break;
            case TOKEN_DIFFERENT:       resultat = builder.CreateFCmpONE(valGauche, valDroite, "fcmp_ne"); typeResultat = llvm::Type::getInt1Ty(builder.getContext()); break;
            default: break;
        }
    } 
    else {
        llvm::Type* intTy = llvm::Type::getInt32Ty(_contextGenCode->backend->getContext());
        typeResultat = intTy;

        switch (noeud->getToken().type) {
            case TOKEN_PLUS:  resultat = builder.CreateAdd(valGauche, valDroite, "iadd"); break;
            case TOKEN_MOINS: resultat = builder.CreateSub(valGauche, valDroite, "isub"); break;
            case TOKEN_ETOILE:resultat = builder.CreateMul(valGauche, valDroite, "imul"); break;
            case TOKEN_SLASH: resultat = builder.CreateSDiv(valGauche, valDroite, "idiv"); break;
            case TOKEN_MODULO: resultat = builder.CreateSRem(valGauche, valDroite, "irem"); break;
            
            // Comparaisons ENTIERS (ICmp)
            case TOKEN_PLUS_PETIT:      resultat = builder.CreateICmpSLT(valGauche, valDroite, "icmp_lt"); typeResultat = llvm::Type::getInt1Ty(builder.getContext()); break;
            case TOKEN_PLUS_GRAND:      resultat = builder.CreateICmpSGT(valGauche, valDroite, "icmp_gt"); typeResultat = llvm::Type::getInt1Ty(builder.getContext()); break;
            case TOKEN_PLUS_PETIT_EGAL: resultat = builder.CreateICmpSLE(valGauche, valDroite, "icmp_le"); typeResultat = llvm::Type::getInt1Ty(builder.getContext()); break;
            case TOKEN_PLUS_GRAND_EGAL: resultat = builder.CreateICmpSGE(valGauche, valDroite, "icmp_ge"); typeResultat = llvm::Type::getInt1Ty(builder.getContext()); break;
            case TOKEN_EGAL_EGAL:       resultat = builder.CreateICmpEQ(valGauche, valDroite, "icmp_eq"); typeResultat = llvm::Type::getInt1Ty(builder.getContext()); break;
            case TOKEN_DIFFERENT:       resultat = builder.CreateICmpNE(valGauche, valDroite, "icmp_ne"); typeResultat = llvm::Type::getInt1Ty(builder.getContext()); break;
            case TOKEN_ET:              resultat = builder.CreateAnd(valGauche, valDroite, "and"); typeResultat = llvm::Type::getInt1Ty(builder.getContext()); break;
            case TOKEN_OU:              resultat = builder.CreateOr(valGauche, valDroite, "or"); typeResultat = llvm::Type::getInt1Ty(builder.getContext()); break;
            default: break;
        }
    }

    resultat = ErrorHelper::verifierNonNull(resultat, "Opération inconnue");

    _contextGenCode->valeurTemporaire.adresse = resultat;
    _contextGenCode->valeurTemporaire.type = new (_contextGenCode->arena->Allocate<TypeSimple>()) TypeSimple(typeResultat);
}