#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/LLVM/GestionVariable.h"
#include "Compilateur/AST/Registre/RegistreClass.h"
#include "Compilateur/Visiteur/CodeGen/Helper/ErrorHelper.h"
#include <llvm/IR/Instructions.h>
#include <string>

void VisiteurGeneralGenCode::visiter(NoeudAccesAttribut* noeudAccesAttribut)
{
    std::string nomObjet = noeudAccesAttribut->getNomObjet().value;
    std::string nomAttribut = noeudAccesAttribut->getNomAttribut().value;

    ChargeurVariable chargeur(_contextGenCode);
    Symbole objetSymbole = chargeur.charger(nomObjet);
    llvm::Value* objet = objetSymbole.getAdresse();

    std::string nomClasse = obtenirNomClasseDepuisSymbole(objetSymbole);

    if (nomClasse.empty()) {
        ErrorHelper::erreurCompilation("Impossible de déterminer la classe de l'objet '" + nomObjet + "'");
    }

    auto* classInfo = _contextGenCode->getRegistreClass()->recuperer(nomClasse).get();
    classInfo = ErrorHelper::verifierNonNull(classInfo, "Classe '" + nomClasse + "' non trouvée dans le registre");

    if (!classInfo->getRegistreVariable()->existeVariable(nomAttribut)) {
        ErrorHelper::erreurCompilation("Attribut '" + nomAttribut + "' inexistant dans la classe '" + nomClasse + "'");
    }

    auto iterator = classInfo->getMemberIndices().find(nomAttribut);
    if (iterator == classInfo->getMemberIndices().end()) {
        ErrorHelper::erreurCompilation("Attribut '" + nomAttribut + "' n'a pas d'index dans " + nomClasse);
    }
    
    unsigned int index = iterator->second;

    auto& builder = _contextGenCode->getBackend()->getBuilder();

    Symbole symboleVar = classInfo->getRegistreVariable()->recupererVariables(noeudAccesAttribut->getNomAttribut());
    
    llvm::Type* typeDuStruct = classInfo->getStructType();

    llvm::Value* attributPtr = builder.CreateStructGEP(typeDuStruct, objet, index, nomObjet + "_" + nomAttribut + "_ptr");

    llvm::Value* valeurAttribut = builder.CreateLoad(
        symboleVar.getType()->genererTypeLLVM(_contextGenCode->getBackend()->getContext()), 
        attributPtr, 
        nomObjet + "_" + nomAttribut
    );

    _contextGenCode->modifierValeurTemporaire(Symbole(valeurAttribut, symboleVar.getType(), builder.getPtrTy()));
}
