#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/LLVM/GestionVariable.h"
#include "Compilateur/AST/Registre/RegistreClass.h"
#include "Compilateur/Visiteur/CodeGen/Helper/ErrorHelper.h"
#include <llvm/IR/Instructions.h>

void VisiteurGeneralGenCode::visiter(NoeudAppelObjet* noeudAppelObjet)
{
    // Récupérer le nom de l'objet (ex: "chien")
    std::string nomObjet = noeudAppelObjet->getNomObjet().value;
    
    // Récupérer la méthode à appeler (ex: "crier")
    std::string nomMethode = noeudAppelObjet->getNomMethode().value;

    ChargeurVariable chargeur(_contextGenCode);
    
    // Charger la variable objet (qui est un pointeur)
    Symbole objetSymbole = chargeur.charger(nomObjet);
    llvm::Value* objet = objetSymbole.adresse;

    std::string nomClasse = obtenirNomClasseDepuisSymbole(objetSymbole);

    if (nomClasse.empty()) {
        ErrorHelper::erreurCompilation("Classe de l'objet '" + nomObjet + "' indéterminée");
    }

    auto* classInfo = _contextGenCode->registreClass->recuperer(nomClasse);
    classInfo = ErrorHelper::verifierNonNull(classInfo, "Classe '" + nomClasse + "' introuvable");

    ErrorHelper::verifierExistence(*classInfo->registreFonctionLocale, nomMethode,
        "Méthode '" + nomMethode + "' inexistante dans '" + nomClasse + "'");

    auto& builder = _contextGenCode->backend->getBuilder();
    
    // Le premier argument à passer à la méthode est le pointeur "this" (qui est l'objet lui-même)
    std::vector<llvm::Value*> args;
    args.push_back(objet);

    const auto& symbolePtr = classInfo->registreFonctionLocale->recuperer(nomMethode);
    const SymboleFonctionLocale* symboleFonction = static_cast<const SymboleFonctionLocale*>(symbolePtr.get());
    llvm::FunctionType* typeFonction = symboleFonction->fonction->getFunctionType();

    unsigned int indexParam = 1; // 0 est "this"
    for (INoeud* argumentEnfant : noeudAppelObjet->getEnfants()) 
    {
        argumentEnfant->accept(this);
        llvm::Value* valeurArgument = _contextGenCode->valeurTemporaire.adresse;
        
        valeurArgument = ErrorHelper::verifierNonNull(valeurArgument, "L'argument d'appel d'objet n'a pas généré de valeur");

        llvm::Value* valeurFinale = valeurArgument;
        if (indexParam < typeFonction->getNumParams()) {
            llvm::Type* typeAttendu = typeFonction->getParamType(indexParam);
            valeurFinale = _contextGenCode->backend->creerAutoCast(valeurArgument, typeAttendu);
        }
        args.push_back(valeurFinale);
        indexParam++;
    }

    builder.CreateCall(symboleFonction->fonction, args);
}
