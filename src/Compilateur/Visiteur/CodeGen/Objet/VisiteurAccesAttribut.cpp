#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/LLVM/GestionVariable.h"
#include "Compilateur/AST/Registre/RegistreClass.h"
#include "Compilateur/Visiteur/CodeGen/Helper/ErrorHelper.h"
#include <llvm/IR/Instructions.h>

void VisiteurGeneralGenCode::visiter(NoeudAccesAttribut* noeudAccesAttribut)
{
    std::string nomObjet = noeudAccesAttribut->getNomObjet().value;
    std::string nomAttribut = noeudAccesAttribut->getNomAttribut().value;

    ChargeurVariable chargeur(_contextGenCode);
    Symbole objetSymbole = chargeur.charger(nomObjet);
    llvm::Value* objet = objetSymbole.adresse;

    std::string nomClasse = obtenirNomClasseDepuisSymbole(objetSymbole);

    if (nomClasse.empty()) {
        ErrorHelper::erreurCompilation("Impossible de déterminer la classe de l'objet '" + nomObjet + "'");
    }

    auto* classInfo = _contextGenCode->registreClass->recuperer(nomClasse);
    classInfo = ErrorHelper::verifierNonNull(classInfo, "Classe '" + nomClasse + "' non trouvée dans le registre");

    if (!classInfo->registreVariable->existeVariable(nomAttribut)) {
        ErrorHelper::erreurCompilation("Attribut '" + nomAttribut + "' inexistant dans la classe '" + nomClasse + "'");
    }

    auto iterator = classInfo->memberIndices.find(nomAttribut);
    if (iterator == classInfo->memberIndices.end()) {
        ErrorHelper::erreurCompilation("Attribut '" + nomAttribut + "' n'a pas d'index dans " + nomClasse);
    }
    
    unsigned int index = iterator->second;

    auto& builder = _contextGenCode->backend->getBuilder();

    Symbole symboleVar = classInfo->registreVariable->recupererVariables(noeudAccesAttribut->getNomAttribut());
    
    llvm::Type* typeDuStruct = classInfo->structType;

    llvm::Value* attributPtr = builder.CreateStructGEP(typeDuStruct, objet, index, nomObjet + "_" + nomAttribut + "_ptr");

    llvm::Value* valeurAttribut = builder.CreateLoad(
        symboleVar.type->genererTypeLLVM(_contextGenCode->backend->getContext()), 
        attributPtr, 
        nomObjet + "_" + nomAttribut
    );

    _contextGenCode->valeurTemporaire = Symbole(valeurAttribut, symboleVar.type, builder.getPtrTy());
}
