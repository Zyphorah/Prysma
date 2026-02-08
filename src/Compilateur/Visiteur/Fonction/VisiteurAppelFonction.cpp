#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/Noeuds/Fonction/NoeudAppelFonction.h"


void VisiteurGeneralGenCode::visiter(NoeudAppelFonction* noeudAppelFonction)
{

    _contextGenCode->registreArgument->vider();
    
    for (const auto& argumentEnfant : noeudAppelFonction->getEnfants()) 
    {
        argumentEnfant->accept(this);
        llvm::Value* valeurArgument = _contextGenCode->valeurTemporaire;

        if (valeurArgument == nullptr) {
             throw std::runtime_error("Erreur : L'argument passé à la fonction " + noeudAppelFonction->_nomFonction.value + " n'a pas généré de valeur.");
        }

        _contextGenCode->registreArgument->ajouter(valeurArgument);
    }

    llvm::Function* fonction = _contextGenCode->registreFonction->recuperer(noeudAppelFonction->_nomFonction.value); // Assurez-vous d'avoir un getter pour le nom ou utilisez _nomFonction.value

    if (fonction == nullptr) {
        throw std::runtime_error("Fonction introuvable : " + noeudAppelFonction->_nomFonction.value);
    }

    std::vector<llvm::Value*> args = *_contextGenCode->registreArgument->recuperer();

    _contextGenCode->valeurTemporaire = _contextGenCode->backend->getBuilder().CreateCall(
        fonction, 
        args, 
        "resultat_appel"
    );
}