#include "Compiler/AST/Registry/Stack/RegistryVariable.h"
#include "Compiler/Lexer/TokenType.h"
#include "Compiler/Visitor/CodeGen/VisitorGeneralGenCode.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Registry/Types/TypeSimple.h"
#include <llvm-18/llvm/IR/Value.h>
#include <llvm/IR/Type.h>

#include "Compiler/Visitor/CodeGen/Helper/ErrorHelper.h"

void GeneralVisitorGenCode::visiter(NodeOperation* node)
{
    // On évalue la gauche et on SAUVEGARDE le Symbole complet
    node->getGauche()->accept(this);
    Symbole symGauche = _contextGenCode->getValeurTemporaire();

    // On évalue la droite (le valeurTemporaire de gauche n'est plus écrasé)
    node->getDroite()->accept(this);
    Symbole symDroite = _contextGenCode->getValeurTemporaire();

    llvm::Value* valGauche = symGauche.getAdresse();
    llvm::Value* valDroite = symDroite.getAdresse();

    auto& builder = _contextGenCode->getBackend()->getBuilder();
    llvm::Value* resultat = nullptr;
    llvm::Type* typeResultat = nullptr;

    // C'est Prysma qui décide du type, plus LLVM
    bool estFlottant = (symGauche.getType() != nullptr && symGauche.getType()->estFlottant()) || 
                       (symDroite.getType() != nullptr && symDroite.getType()->estFlottant());

    if (estFlottant) {
        llvm::Type* floatTy = llvm::Type::getFloatTy(_contextGenCode->getBackend()->getContext());
        valGauche = _contextGenCode->getBackend()->creerAutoCast(valGauche, floatTy);
        valDroite = _contextGenCode->getBackend()->creerAutoCast(valDroite, floatTy);
        typeResultat = floatTy;

        switch (node->getToken().type) {
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
        llvm::Type* intTy = llvm::Type::getInt32Ty(_contextGenCode->getBackend()->getContext());
        typeResultat = intTy;

        switch (node->getToken().type) {
            case TOKEN_PLUS:  resultat = builder.CreateAdd(valGauche, valDroite, "iadd"); break;
            case TOKEN_MOINS: resultat = builder.CreateSub(valGauche, valDroite, "isub"); break;
            case TOKEN_ETOILE:resultat = builder.CreateMul(valGauche, valDroite, "imul"); break;
            case TOKEN_SLASH: resultat = builder.CreateSDiv(valGauche, valDroite, "idiv"); break;
            case TOKEN_MODULO: resultat = builder.CreateSRem(valGauche, valDroite, "irem"); break;
            
            // Comparisons ENTIERS (ICmp)
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

    _contextGenCode->modifierValeurTemporaire(Symbole(resultat, _contextGenCode->getValeurTemporaire().getType(), _contextGenCode->getValeurTemporaire().getTypePointeElement()));
    _contextGenCode->modifierValeurTemporaire(Symbole(_contextGenCode->getValeurTemporaire().getAdresse(), new (_contextGenCode->getArena()->Allocate<TypeSimple>()) TypeSimple(typeResultat), _contextGenCode->getValeurTemporaire().getTypePointeElement()));
}