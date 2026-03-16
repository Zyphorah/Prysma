#include "Compilateur/AST/Utils/ConstructeurEnvironnementRegistreFonction.h"
#include "Compilateur/AST/Registre/RegistreFonction.h"
#include "Compilateur/AST/Registre/RegistreClass.h"
#include "Compilateur/AST/AST_Genere.h"

ConstructeurEnvironnementRegistreFonction::ConstructeurEnvironnementRegistreFonction(ContextGenCode* contextGenCode)
    : _contextGenCode(contextGenCode) {}

ConstructeurEnvironnementRegistreFonction::~ConstructeurEnvironnementRegistreFonction() {}

void ConstructeurEnvironnementRegistreFonction::remplir()
{   
    // 1. Remplissage des fonctions globales
    for(const auto& cle : _contextGenCode->registreFonctionGlobale->obtenirCles())
    {
        const auto& ancienSymboleUniquePtr = _contextGenCode->registreFonctionGlobale->recuperer(cle);
        const auto* ancienSymbole = static_cast<const SymboleFonctionGlobale*>(ancienSymboleUniquePtr.get());
        
        if (ancienSymbole->noeud == nullptr) continue;

        llvm::Type* retType = ancienSymbole->typeRetour->genererTypeLLVM(_contextGenCode->backend->getContext());
        
        std::vector<llvm::Type*> paramTypes;
        for (auto* arg : ancienSymbole->noeud->getArguments()) {
            auto* argFonction = static_cast<NoeudArgFonction*>(arg);
            paramTypes.push_back(argFonction->getType()->genererTypeLLVM(_contextGenCode->backend->getContext()));
        }

        llvm::FunctionType* funcType = llvm::FunctionType::get(retType, paramTypes, false);
        
        llvm::Function* vraieFonction = llvm::Function::Create(
            funcType, 
            llvm::Function::ExternalLinkage, 
            cle,          
            _contextGenCode->backend->getModule()
        );

        auto nouveauSymbole = std::make_unique<SymboleFonctionLocale>();
        nouveauSymbole->fonction = vraieFonction;
        nouveauSymbole->typeRetour = ancienSymbole->typeRetour;
        nouveauSymbole->noeud = ancienSymbole->noeud;
        
        _contextGenCode->registreFonctionLocale->enregistrer(cle, std::move(nouveauSymbole));        
    }

    // Remplissage des méthodes de classes (VTable, mangling)
    for (const auto& nomClasse : _contextGenCode->registreClass->obtenirCles())
    {
        auto* classInfo = _contextGenCode->registreClass->recuperer(nomClasse);
        
        for (const auto& nomMethode : classInfo->registreFonctionLocale->obtenirCles())
        {
            // On récupère le SymboleFonctionLocale créé dans VisiteurRemplissageRegistre
            const auto& symboleUniquePtr = classInfo->registreFonctionLocale->recuperer(nomMethode);
            auto* symbole = static_cast<SymboleFonctionLocale*>(symboleUniquePtr.get());
            
            if (symbole->noeud == nullptr || symbole->fonction != nullptr) continue;

            llvm::Type* retType = symbole->typeRetour->genererTypeLLVM(_contextGenCode->backend->getContext());
            
            std::vector<llvm::Type*> paramTypes;
            // TODO: Ajouter le paramètre 'this' caché comme premier paramètre
            // Ce sera le type pointeur vers la classe : classInfo->structType->getPointerTo()

            for (auto* arg : symbole->noeud->getArguments()) {
                auto* argFonction = static_cast<NoeudArgFonction*>(arg);
                paramTypes.push_back(argFonction->getType()->genererTypeLLVM(_contextGenCode->backend->getContext()));
            }

            llvm::FunctionType* funcType = llvm::FunctionType::get(retType, paramTypes, false);
            
            // Mangling du nom pour LLVM (ex: Joueur_initialiser)
            std::string nomLLVM = nomClasse;
            nomLLVM += "_";
            nomLLVM += nomMethode;

            llvm::Function* vraieFonction = llvm::Function::Create(
                funcType, 
                llvm::Function::ExternalLinkage, 
                nomLLVM,          
                _contextGenCode->backend->getModule()
            );

            symbole->fonction = vraieFonction;     
        }
    }
}
