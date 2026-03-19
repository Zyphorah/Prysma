#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/Visiteur/CodeGen/Helper/ErrorHelper.h"

void VisiteurGeneralGenCode::visiter(NoeudDelete* noeudDelete)
{
    auto& module = _contextGenCode->backend->getModule();
    auto& builder = _contextGenCode->backend->getBuilder();

    // Récupérer le token de la variable à supprimer
    const Token& tokenVariable = noeudDelete->getNomType();

    // Chercher la variable dans le registre de variables pour déterminer si elle existe
    Symbole symbole = _contextGenCode->registreVariable->recupererVariables(tokenVariable);

    ErrorHelper::verifierNonNull(symbole.adresse, "Variable '" + tokenVariable.value + "' non déclarée");
    
    llvm::Value* adresseMemoire = symbole.adresse;
    llvm::Type* typeDonnee = adresseMemoire->getType();

    if (!typeDonnee->isPointerTy()) {
        ErrorHelper::erreurCompilation("Variable '" + tokenVariable.value + "' n'est pas un pointeur (delete requiert un pointeur)");
    }

    llvm::Value* adresseALiberer = builder.CreateLoad(
        typeDonnee,
        adresseMemoire,
        "pointeur_" + tokenVariable.value
    );

    // Récupérer la fonction prysma_free du module
    llvm::Function* freeFunc = module.getFunction("prysma_free");

    freeFunc = ErrorHelper::verifierNonNull(freeFunc, "Fonction prysma_free non déclarée");

    // Appeler prysma_free avec l'adresse à libérer
    builder.CreateCall(freeFunc, {adresseALiberer});

    // Réinitialiser la valeur temporaire
    _contextGenCode->valeurTemporaire.adresse = nullptr;
}
