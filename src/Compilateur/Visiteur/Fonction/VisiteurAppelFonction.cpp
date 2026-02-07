#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"


void VisiteurGeneralGenCode::visiter(NoeudAppelFonction* noeudAppelFonction)
{
    /*
     _registreArgument->vider();
    executerEnfants();
    llvm::Function* fonction = _registreFonction->recuperer(_nomFonction.value);
    std::vector<llvm::Value*> args = *_registreArgument->recuperer();
    std::vector<llvm::Value*> argsConvertieLecture; 

    GestionnaireChargementVariable gestionnaireChargementVariable(_backend);

    for(size_t i = 0; i < args.size(); i++)
    {
        llvm::StringRef nom = args[i]->getName();
        std::string nomConvertieString(nom.begin(), nom.end());
        argsConvertieLecture.push_back(gestionnaireChargementVariable.chargerVariable(args[i], nomConvertieString));
    }

    return _backend->getBuilder().CreateCall(fonction, argsConvertieLecture, "resultat_appel");
    */
}