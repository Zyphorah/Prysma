#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/LLVM/GestionVariable.h"
#include "Compilateur/AST/Registre/RegistreClass.h"
#include "Compilateur/AST/Registre/Types/TypeComplexe.h"
#include <stdexcept>
#include <llvm/IR/Instructions.h>

void VisiteurGeneralGenCode::visiter(NoeudAppelObjet* noeudAppelObjet)
{
    // Récupérer le nom de l'objet (ex: "chien")
    std::string nomObjet = noeudAppelObjet->getNomObjet().value;
    
    // Récupérer la méthode à appeler (ex: "crier")
    std::string nomMethode = noeudAppelObjet->getNomMethode().value;

    GestionVariable gestionVariable(_contextGenCode);
    
    // Charger la variable objet (qui est un pointeur)
    Symbole objetSymbole = gestionVariable.chargerVariable(nomObjet);
    llvm::Value* objet = objetSymbole.adresse;

    std::string nomClasse = "";
    if (objetSymbole.type != nullptr) {
        if (auto* typeComplexe = dynamic_cast<TypeComplexe*>(objetSymbole.type)) {
            nomClasse = typeComplexe->getNomClasse();
        }
    }

    if (nomClasse.empty() && objetSymbole.typePointeElement != nullptr) {
        if (auto* structType = llvm::dyn_cast<llvm::StructType>(objetSymbole.typePointeElement)) {
            llvm::StringRef structName = structType->getName();
            if (structName.starts_with("Class_")) {
                nomClasse = structName.drop_front(6).str();
            } else {
                nomClasse = structName.str();
            }
        }
    }

    if (nomClasse.empty()) {
        throw std::runtime_error("Erreur: impossible de determiner la classe de l'objet " + nomObjet);
    }

    auto* classInfo = _contextGenCode->registreClass->recuperer(nomClasse);
    if (!classInfo) {
        throw std::runtime_error("Erreur: classe " + nomClasse + " non trouvée dans le registre.");
    }

    if (!classInfo->registreFonctionLocale->existe(nomMethode)) {
        throw std::runtime_error("Erreur: la méthode " + nomMethode + " n'existe pas dans la classe " + nomClasse);
    }

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
        
        if (valeurArgument == nullptr) {
            throw std::runtime_error("Erreur : L'argument d'appel d'objet n'a pas généré de valeur.");
        }

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
