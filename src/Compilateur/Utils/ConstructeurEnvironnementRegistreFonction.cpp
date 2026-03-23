#include "Compilateur/AST/Utils/ConstructeurEnvironnementRegistreFonction.h"
#include "Compilateur/AST/Registre/ContextGenCode.h"
#include "Compilateur/AST/Registre/RegistreFonction.h"
#include "Compilateur/AST/Registre/RegistreClass.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/Utils/PrysmaCast.h"
#include "Compilateur/Visiteur/Extracteurs/ExtracteurMembresClass.h"
#include <llvm-18/llvm/IR/Function.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

ConstructeurEnvironnementRegistreFonction::ConstructeurEnvironnementRegistreFonction(ContextGenCode* contextGenCode)
    : _contextGenCode(contextGenCode) {}

ConstructeurEnvironnementRegistreFonction::~ConstructeurEnvironnementRegistreFonction() = default;

void ConstructeurEnvironnementRegistreFonction::construireVTable(Class* classInfo, const std::string& nomClass, const std::vector<NoeudDeclarationFonction*>& listMethodeParent)
{
    // Construire la vtable en alignant les méthodes du parent aux mêmes positions
    std::vector<llvm::Constant*> vtableElements;
    
    // Ajouter les pointeurs de fonction du parent à la vtable, dans le même ordre
    for (NoeudDeclarationFonction* declarationMethodeParent : listMethodeParent) {
        std::string nomMethodeParent = declarationMethodeParent->getNom();
        
        // Chercher la méthode correspondante dans la classe courante
        llvm::Function* functionImpl = nullptr;
        if (classInfo->getRegistreFonctionLocale()->existe(nomMethodeParent)) {
            const auto& symbole = classInfo->getRegistreFonctionLocale()->recuperer(nomMethodeParent);
            if (!prysma::isa<SymboleFonctionLocale>(symbole.get())) {
                throw std::runtime_error("Erreur : SymboleFonctionLocale attendu");
            }
            auto* symboleLocal = prysma::cast<SymboleFonctionLocale>(symbole.get());
            functionImpl = symboleLocal->fonction;
        }
        
        // Si trouvée, ajouter le pointeur de fonction à la vtable
        if (functionImpl != nullptr) {
            vtableElements.push_back(llvm::ConstantExpr::getBitCast(
                functionImpl,
                llvm::PointerType::get(llvm::Type::getInt8Ty(_contextGenCode->getBackend()->getContext()), 0)
            ));
            classInfo->getMethodIndices()[nomMethodeParent] = static_cast<unsigned int>(vtableElements.size() - 1);
        }
    }
    
    // Ajouter les méthodes supplémentaires de la classe courante qui ne sont pas dans le parent
    auto clesMethodes = classInfo->getRegistreFonctionLocale()->obtenirCles();
    for (const auto& cle : clesMethodes) {
        const std::string& nomMethode = cle;
        bool estDansParent = false;
        
        // Vérifier si cette méthode est dans le parent
        for (NoeudDeclarationFonction* declarationMethodeParent : listMethodeParent) {
            if (declarationMethodeParent->getNom() == nomMethode) {
                estDansParent = true;
                break;
            }
        }
        
        // Si ce n'est pas une méthode du parent, l'ajouter à la fin de la vtable
        if (!estDansParent) {
            const auto& symbole = classInfo->getRegistreFonctionLocale()->recuperer(nomMethode);
            if (!prysma::isa<SymboleFonctionLocale>(symbole.get())) {
                throw std::runtime_error("Erreur : SymboleFonctionLocale attendu");
            }
            auto* symboleLocal = prysma::cast<SymboleFonctionLocale>(symbole.get()); 
            if (symboleLocal->fonction != nullptr) {
                vtableElements.push_back(llvm::ConstantExpr::getBitCast(
                    symboleLocal->fonction,
                    llvm::PointerType::get(llvm::Type::getInt8Ty(_contextGenCode->getBackend()->getContext()), 0)
                ));
                classInfo->getMethodIndices()[nomMethode] = static_cast<unsigned int>(vtableElements.size() - 1);
            }
        }
    }
    
    // Créer un array constant pour la vtable
    llvm::ArrayType* vtableType = llvm::ArrayType::get(
        llvm::PointerType::get(llvm::Type::getInt8Ty(_contextGenCode->getBackend()->getContext()), 0),
        vtableElements.size()
    );
    llvm::Constant* vtableInitializer = llvm::ConstantArray::get(vtableType, vtableElements);
    
    // Créer une variable globale pour la vtable
    llvm::Module* module = &(_contextGenCode->getBackend()->getModule());
    classInfo->setVTable(new llvm::GlobalVariable(
        *module,
        vtableType,
        true,  
        llvm::GlobalValue::LinkageTypes::InternalLinkage,
        vtableInitializer,
        "vtable_" + nomClass
    )); 
}


void ConstructeurEnvironnementRegistreFonction::remplir()
{   
    // 1. Remplissage des fonctions globales
    for(const auto& cle : _contextGenCode->getRegistreFonctionGlobale()->obtenirCles())
    {
        const auto& ancienSymboleUniquePtr = _contextGenCode->getRegistreFonctionGlobale()->recuperer(cle);
        const auto* ancienSymbole = prysma::cast<const SymboleFonctionGlobale>(ancienSymboleUniquePtr.get());
        
        if (ancienSymbole->noeud == nullptr) { continue;}

        llvm::Type* retType = ancienSymbole->typeRetour->genererTypeLLVM(_contextGenCode->getBackend()->getContext());
        
        std::vector<llvm::Type*> paramTypes;
        for (auto* arg : ancienSymbole->noeud->getArguments()) {
            auto* argFonction = prysma::cast<NoeudArgFonction>(arg);
            paramTypes.push_back(argFonction->getType()->genererTypeLLVM(_contextGenCode->getBackend()->getContext()));
        }

        llvm::FunctionType* funcType = llvm::FunctionType::get(retType, paramTypes, false);
        
        llvm::Function* vraieFonction = llvm::Function::Create(
            funcType, 
            llvm::Function::ExternalLinkage, 
            cle,          
            _contextGenCode->getBackend()->getModule()
        );

        auto nouveauSymbole = std::make_unique<SymboleFonctionLocale>();
        nouveauSymbole->fonction = vraieFonction;
        nouveauSymbole->typeRetour = ancienSymbole->typeRetour;
        nouveauSymbole->noeud = ancienSymbole->noeud;
        
        _contextGenCode->getRegistreFonctionLocale()->enregistrer(cle, std::move(nouveauSymbole));        
    }

    // Remplissage des méthodes de classes (VTable, mangling)
    for (const auto& nomClasse : _contextGenCode->getRegistreClass()->obtenirCles())
    {
        auto const& classInfo = _contextGenCode->getRegistreClass()->recuperer(nomClasse);
        
        for (const auto& nomMethode : classInfo->getRegistreFonctionLocale()->obtenirCles())
        {
            // On récupère le SymboleFonctionLocale créé dans VisiteurRemplissageRegistre
            const auto& symboleUniquePtr = classInfo->getRegistreFonctionLocale()->recuperer(nomMethode);
            auto* symbole = prysma::cast<SymboleFonctionLocale>(symboleUniquePtr.get());
            
            if (symbole->noeud == nullptr || symbole->fonction != nullptr) 
            {
                std::string errorMsg = "Erreur : SymboleFonctionLocale pour la méthode '";
                errorMsg += nomMethode;
                errorMsg += "' de la classe '";
                errorMsg += nomClasse;
                errorMsg += "' est mal initialisé ou déjà traité";
                throw std::runtime_error(errorMsg);
            }

            llvm::Type* retType = symbole->typeRetour->genererTypeLLVM(_contextGenCode->getBackend()->getContext());
            
            std::vector<llvm::Type*> paramTypes;
            
            // Ajouter le paramètre 'this' caché comme premier paramètre
            paramTypes.push_back(llvm::PointerType::getUnqual(_contextGenCode->getBackend()->getContext()));

            for (auto* arg : symbole->noeud->getArguments()) {
                auto* argFonction = prysma::cast<NoeudArgFonction>(arg);
                paramTypes.push_back(argFonction->getType()->genererTypeLLVM(_contextGenCode->getBackend()->getContext()));
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
                _contextGenCode->getBackend()->getModule()
            );
            
            symbole->fonction = vraieFonction;    
            
            // Construire la vtable pour cette classe
            std::vector<NoeudDeclarationFonction*> listMethodeParent;
            if (classInfo->getParentHeritage() != nullptr) {
                ExtracteurMembresClass extracteurParent;
                classInfo->getParentHeritage()->accept(&extracteurParent);
                listMethodeParent = extracteurParent.getMethodes();
            }
            construireVTable(classInfo.get(), nomClasse, listMethodeParent);
        }
    }
}
