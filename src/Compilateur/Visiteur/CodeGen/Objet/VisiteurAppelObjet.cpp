#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include "Compilateur/AST/Registre/RegistreFonction.h"
#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/LLVM/GestionVariable.h"
#include "Compilateur/AST/Registre/RegistreClass.h"
#include "Compilateur/Visiteur/CodeGen/Helper/ErrorHelper.h"
#include "Compilateur/Visiteur/CodeGen/Helper/VTableNavigator.h"
#include "Compilateur/Utils/PrysmaCast.h"
#include <llvm-18/llvm/IR/Value.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/Casting.h>
#include <stdexcept>
#include <string>
#include <vector>

void VisiteurGeneralGenCode::visiter(NoeudAppelObjet* noeudAppelObjet)
{
    // Récupérer le nom de l'objet (ex: "chien")
    std::string nomObjet = noeudAppelObjet->getNomObjet().value;

    // Récupérer le nom de la méthode appelée (ex: "aboyer")
    std::string nomMethode = noeudAppelObjet->getNomMethode().value;

    ChargeurVariable chargeur(_contextGenCode);
    
    // Charger la variable objet (qui est un pointeur)
    Symbole objetSymbole = chargeur.charger(nomObjet);
    llvm::Value* objet = objetSymbole.getAdresse();

    std::string nomClasse = obtenirNomClasseDepuisSymbole(objetSymbole);

    if (nomClasse.empty()) {
        ErrorHelper::erreurCompilation("Classe de l'objet '" + nomObjet + "' indéterminée");
    }
    // On crée le chemin vers l'objet 
    auto* typeStructClass = llvm::dyn_cast<llvm::StructType>(objetSymbole.getTypePointeElement());
    llvm::Value* adresseVptr = _contextGenCode->getBackend()->getBuilder().CreateStructGEP(typeStructClass, objet, 0, "adresse_vptr");

    
    // Récupérer la vtable de la classe qui contient les méthodes de l'objet (ex: "Chien")
    // On dois construire la vtable de façon dynamique à partir de l'adresse du vptr dans l'objet, car en cas d'héritage la vtable peut être différente 
    // de celle de la classe static de l'objet 
    llvm::Value* vtable = _contextGenCode->getBackend()->getBuilder().CreateLoad(_contextGenCode->getBackend()->getBuilder().getPtrTy(), adresseVptr, "vptr");
    // On récupère l'index de la méthode dans la vtable pour faire le call indirect
    int indexMethode = _contextGenCode->getRegistreClass()->recuperer(nomClasse)->getIndexMethode(nomMethode);

    auto* classInfo = _contextGenCode->getRegistreClass()->recuperer(nomClasse).get();
    classInfo = ErrorHelper::verifierNonNull(classInfo, "Classe '" + nomClasse + "' introuvable");

    ErrorHelper::verifierExistence(*classInfo->getRegistreFonctionLocale(), nomMethode,
        "Méthode '" + nomMethode + "' inexistante dans '" + nomClasse + "'");

    auto& builder = _contextGenCode->getBackend()->getBuilder();
    
    VTableNavigator navigator(&builder);
    
    llvm::Value* methodePointeur = navigator.recupererPointeurMethode(vtable, classInfo->getVTable()->getValueType(), indexMethode);
    
    // Le premier argument à passer à la méthode est le pointeur "this" (qui est l'objet lui-même)
    std::vector<llvm::Value*> args;
    args.push_back(objet);

    const auto& symbolePtr = classInfo->getRegistreFonctionLocale()->recuperer(nomMethode);
    if (!prysma::isa<SymboleFonctionLocale>(symbolePtr.get())) {
        throw std::runtime_error("Erreur : SymboleFonctionLocale attendu");
    }
    const auto* symboleFonction = prysma::cast<const SymboleFonctionLocale>(symbolePtr.get());
    llvm::FunctionType* typeFonction = symboleFonction->fonction->getFunctionType();

    unsigned int indexParam = 1; // 0 est "this"
    for (INoeud* argumentEnfant : noeudAppelObjet->getEnfants()) 
    {
        argumentEnfant->accept(this);
        llvm::Value* valeurArgument = _contextGenCode->getValeurTemporaire().getAdresse();
        
        valeurArgument = ErrorHelper::verifierNonNull(valeurArgument, "L'argument d'appel d'objet n'a pas généré de valeur");

        llvm::Value* valeurFinale = valeurArgument;
        if (indexParam < typeFonction->getNumParams()) {
            llvm::Type* typeAttendu = typeFonction->getParamType(indexParam);
            valeurFinale = _contextGenCode->getBackend()->creerAutoCast(valeurArgument, typeAttendu);
        }
        args.push_back(valeurFinale);
        indexParam++;
    }

    // Si la méthode est le constructeur alors il faut appeler la méthode d'initialisation static 
    if(nomMethode == nomClasse)
    {
        builder.CreateCall(symboleFonction->fonction, args);
        return;
    }

    // On utilise la vtable pour faire un call indirect à la méthode
    if (symboleFonction->fonction->getReturnType()->isVoidTy()) { 
         builder.CreateCall(typeFonction, methodePointeur, args);
        _contextGenCode->modifierValeurTemporaire(Symbole(nullptr, _contextGenCode->getValeurTemporaire().getType(), _contextGenCode->getValeurTemporaire().getTypePointeElement()));
        _contextGenCode->modifierValeurTemporaire(Symbole(_contextGenCode->getValeurTemporaire().getAdresse(), nullptr, _contextGenCode->getValeurTemporaire().getTypePointeElement()));
    } else {
        llvm::Value* resultat = builder.CreateCall(typeFonction, methodePointeur, args, "resultat_appel_objet");
        _contextGenCode->modifierValeurTemporaire(Symbole(resultat, _contextGenCode->getValeurTemporaire().getType(), _contextGenCode->getValeurTemporaire().getTypePointeElement()));
        _contextGenCode->modifierValeurTemporaire(Symbole(_contextGenCode->getValeurTemporaire().getAdresse(), symboleFonction->typeRetour, _contextGenCode->getValeurTemporaire().getTypePointeElement()));
    }
}
