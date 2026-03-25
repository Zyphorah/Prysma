#include "Compiler/AST/Registry/Stack/RegistryVariable.h"
#include "Compiler/AST/Registry/RegistryFunction.h"
#include "Compiler/Visitor/CodeGen/VisitorGeneralGenCode.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/LLVM/GestionVariable.h"
#include "Compiler/AST/Registry/RegistryClass.h"
#include "Compiler/Visitor/CodeGen/Helper/ErrorHelper.h"
#include "Compiler/Visitor/CodeGen/Helper/VTableNavigator.h"
#include "Compiler/Utils/PrysmaCast.h"
#include <llvm-18/llvm/IR/Value.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/Casting.h>
#include <stdexcept>
#include <string>
#include <vector>

void GeneralVisitorGenCode::visiter(NodeCallObject* nodeCallObject)
{
    // Récupérer le nom de l'object (ex: "chien")
    std::string nomObject = nodeCallObject->getNomObject().value;

    // Récupérer le nom de la méthode callée (ex: "aboyer")
    std::string nomMethode = nodeCallObject->getNomMethode().value;

    LoadurVariable loadur(_contextGenCode);
    
    // Loadr la variable object (qui est un pointeur)
    Symbole objectSymbole = loadur.loadr(nomObject);
    llvm::Value* object = objectSymbole.getAdresse();

    std::string nomClasse = obtenirNomClasseDepuisSymbole(objectSymbole);

    if (nomClasse.empty()) {
        ErrorHelper::errorCompilation("Classe de l'object '" + nomObject + "' indéterminée");
    }
    // On crée le chemin vers l'object 
    auto* typeStructClass = llvm::dyn_cast<llvm::StructType>(objectSymbole.getTypePointeElement());
    llvm::Value* adresseVptr = _contextGenCode->getBackend()->getBuilder().CreateStructGEP(typeStructClass, object, 0, "adresse_vptr");

    
    // Récupérer la vtable de la classe qui contient les méthodes de l'object (ex: "Chien")
    // On dois construire la vtable de façon dynamique à partir de l'adresse du vptr dans l'object, car en cas d'héritage la vtable peut être différente 
    // de celle de la classe static de l'object 
    llvm::Value* vtable = _contextGenCode->getBackend()->getBuilder().CreateLoad(_contextGenCode->getBackend()->getBuilder().getPtrTy(), adresseVptr, "vptr");
    // On récupère l'index de la méthode dans la vtable pour faire le call indirect
    int indexMethode = _contextGenCode->getRegistryClass()->recuperer(nomClasse)->getIndexMethode(nomMethode);

    auto* classInfo = _contextGenCode->getRegistryClass()->recuperer(nomClasse).get();
    classInfo = ErrorHelper::verifierNonNull(classInfo, "Classe '" + nomClasse + "' introuvable");

    ErrorHelper::verifierExistence(*classInfo->getRegistryFunctionLocale(), nomMethode,
        "Méthode '" + nomMethode + "' inexistante dans '" + nomClasse + "'");

    auto& builder = _contextGenCode->getBackend()->getBuilder();
    
    VTableNavigator navigator(&builder);
    
    llvm::Value* methodePointer = navigator.recupererPointerMethode(vtable, classInfo->getVTable()->getValueType(), indexMethode);
    
    // Le premier argument à passer à la méthode est le pointeur "this" (qui est l'object lui-même)
    std::vector<llvm::Value*> args;
    args.push_back(object);

    const auto& symbolePtr = classInfo->getRegistryFunctionLocale()->recuperer(nomMethode);
    if (!prysma::isa<SymboleFunctionLocale>(symbolePtr.get())) {
        throw std::runtime_error("Error : SymboleFunctionLocale attendu");
    }
    const auto* symboleFunction = prysma::cast<const SymboleFunctionLocale>(symbolePtr.get());
    llvm::FunctionType* typeFunction = symboleFunction->function->getFunctionType();

    unsigned int indexParam = 1; // 0 est "this"
    for (INode* argumentChild : nodeCallObject->getChilds()) 
    {
        argumentChild->accept(this);
        llvm::Value* valeurArgument = _contextGenCode->getValeurTemporaire().getAdresse();
        
        valeurArgument = ErrorHelper::verifierNonNull(valeurArgument, "L'argument d'call d'object n'a pas généré de valeur");

        llvm::Value* valeurFinale = valeurArgument;
        if (indexParam < typeFunction->getNumParams()) {
            llvm::Type* typeAttendu = typeFunction->getParamType(indexParam);
            valeurFinale = _contextGenCode->getBackend()->creerAutoCast(valeurArgument, typeAttendu);
        }
        args.push_back(valeurFinale);
        indexParam++;
    }

    // Si la méthode est le builder alors il faut caller la méthode d'initialization static 
    if(nomMethode == nomClasse)
    {
        builder.CreateCall(symboleFunction->function, args);
        return;
    }

    // On utilise la vtable pour faire un call indirect à la méthode
    if (symboleFunction->function->getReturnType()->isVoidTy()) { 
         builder.CreateCall(typeFunction, methodePointer, args);
        _contextGenCode->modifierValeurTemporaire(Symbole(nullptr, _contextGenCode->getValeurTemporaire().getType(), _contextGenCode->getValeurTemporaire().getTypePointeElement()));
        _contextGenCode->modifierValeurTemporaire(Symbole(_contextGenCode->getValeurTemporaire().getAdresse(), nullptr, _contextGenCode->getValeurTemporaire().getTypePointeElement()));
    } else {
        llvm::Value* resultat = builder.CreateCall(typeFunction, methodePointer, args, "resultat_call_object");
        _contextGenCode->modifierValeurTemporaire(Symbole(resultat, _contextGenCode->getValeurTemporaire().getType(), _contextGenCode->getValeurTemporaire().getTypePointeElement()));
        _contextGenCode->modifierValeurTemporaire(Symbole(_contextGenCode->getValeurTemporaire().getAdresse(), symboleFunction->typeReturn, _contextGenCode->getValeurTemporaire().getTypePointeElement()));
    }
}
