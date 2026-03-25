#include "Compiler/AST/Utils/BuilderEnvironmentRegistryFunction.h"
#include "Compiler/AST/Registry/ContextGenCode.h"
#include "Compiler/AST/Registry/RegistryFunction.h"
#include "Compiler/AST/Registry/RegistryClass.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/Utils/PrysmaCast.h"
#include "Compiler/Visitor/Extractors/MembersExtractorClass.h"
#include <llvm-18/llvm/IR/Function.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

BuilderEnvironmentRegistryFunction::BuilderEnvironmentRegistryFunction(ContextGenCode* contextGenCode)
    : _contextGenCode(contextGenCode) {}

BuilderEnvironmentRegistryFunction::~BuilderEnvironmentRegistryFunction() = default;

void BuilderEnvironmentRegistryFunction::construireVTable(Class* classInfo, const std::string& nomClass, const std::vector<NodeDeclarationFunction*>& listMethodeParent)
{
    // Construire la vtable en alignant les méthodes du parent aux mêmes positions
    std::vector<llvm::Constant*> vtableElements;
    
    // Ajouter les pointeurs de function du parent à la vtable, dans le même ordre
    for (NodeDeclarationFunction* declarationMethodeParent : listMethodeParent) {
        std::string nomMethodeParent = declarationMethodeParent->getNom();
        
        // Chercher la méthode correspondante dans la classe courante
        llvm::Function* functionImpl = nullptr;
        if (classInfo->getRegistryFunctionLocale()->existe(nomMethodeParent)) {
            const auto& symbole = classInfo->getRegistryFunctionLocale()->recuperer(nomMethodeParent);
            if (!prysma::isa<SymboleFunctionLocale>(symbole.get())) {
                throw std::runtime_error("Error : SymboleFunctionLocale attendu");
            }
            auto* symboleLocal = prysma::cast<SymboleFunctionLocale>(symbole.get());
            functionImpl = symboleLocal->function;
        }
        
        // Si trouvée, ajouter le pointeur de function à la vtable
        if (functionImpl != nullptr) {
            vtableElements.push_back(llvm::ConstantExpr::getBitCast(
                functionImpl,
                llvm::PointerType::get(llvm::Type::getInt8Ty(_contextGenCode->getBackend()->getContext()), 0)
            ));
            classInfo->getMethodIndices()[nomMethodeParent] = static_cast<unsigned int>(vtableElements.size() - 1);
        }
    }
    
    // Ajouter les méthodes supplémentaires de la classe courante qui ne sont pas dans le parent
    auto clesMethodes = classInfo->getRegistryFunctionLocale()->obtenirCles();
    for (const auto& cle : clesMethodes) {
        const std::string& nomMethode = cle;
        bool estDansParent = false;
        
        // Vérifier si cette méthode est dans le parent
        for (NodeDeclarationFunction* declarationMethodeParent : listMethodeParent) {
            if (declarationMethodeParent->getNom() == nomMethode) {
                estDansParent = true;
                break;
            }
        }
        
        // Si ce n'est pas une méthode du parent, l'ajouter à la fin de la vtable
        if (!estDansParent) {
            const auto& symbole = classInfo->getRegistryFunctionLocale()->recuperer(nomMethode);
            if (!prysma::isa<SymboleFunctionLocale>(symbole.get())) {
                throw std::runtime_error("Error : SymboleFunctionLocale attendu");
            }
            auto* symboleLocal = prysma::cast<SymboleFunctionLocale>(symbole.get()); 
            if (symboleLocal->function != nullptr) {
                vtableElements.push_back(llvm::ConstantExpr::getBitCast(
                    symboleLocal->function,
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


void BuilderEnvironmentRegistryFunction::remplir()
{   
    // 1. Filling des functions globales
    for(const auto& cle : _contextGenCode->getRegistryFunctionGlobale()->obtenirCles())
    {
        const auto& ancienSymboleUniquePtr = _contextGenCode->getRegistryFunctionGlobale()->recuperer(cle);
        const auto* ancienSymbole = prysma::cast<const SymboleFunctionGlobale>(ancienSymboleUniquePtr.get());
        
        if (ancienSymbole->node == nullptr) { continue;}

        llvm::Type* retType = ancienSymbole->typeReturn->generatedrTypeLLVM(_contextGenCode->getBackend()->getContext());
        
        std::vector<llvm::Type*> paramTypes;
        for (auto* arg : ancienSymbole->node->getArguments()) {
            auto* argFunction = prysma::cast<NodeArgFunction>(arg);
            paramTypes.push_back(argFunction->getType()->generatedrTypeLLVM(_contextGenCode->getBackend()->getContext()));
        }

        llvm::FunctionType* funcType = llvm::FunctionType::get(retType, paramTypes, false);
        
        llvm::Function* vraieFunction = llvm::Function::Create(
            funcType, 
            llvm::Function::ExternalLinkage, 
            cle,          
            _contextGenCode->getBackend()->getModule()
        );

        auto nouveauSymbole = std::make_unique<SymboleFunctionLocale>();
        nouveauSymbole->function = vraieFunction;
        nouveauSymbole->typeReturn = ancienSymbole->typeReturn;
        nouveauSymbole->node = ancienSymbole->node;
        
        _contextGenCode->getRegistryFunctionLocale()->enregistryr(cle, std::move(nouveauSymbole));        
    }

    // Filling des méthodes de classes (VTable, mangling)
    for (const auto& nomClasse : _contextGenCode->getRegistryClass()->obtenirCles())
    {
        auto const& classInfo = _contextGenCode->getRegistryClass()->recuperer(nomClasse);
        
        for (const auto& nomMethode : classInfo->getRegistryFunctionLocale()->obtenirCles())
        {
            // On récupère le SymboleFunctionLocale créé dans FillingVisitorRegistry
            const auto& symboleUniquePtr = classInfo->getRegistryFunctionLocale()->recuperer(nomMethode);
            auto* symbole = prysma::cast<SymboleFunctionLocale>(symboleUniquePtr.get());
            
            if (symbole->node == nullptr || symbole->function != nullptr) 
            {
                std::string errorMsg = "Error : SymboleFunctionLocale pour la méthode '";
                errorMsg += nomMethode;
                errorMsg += "' de la classe '";
                errorMsg += nomClasse;
                errorMsg += "' est mal initialisé ou déjà traité";
                throw std::runtime_error(errorMsg);
            }

            llvm::Type* retType = symbole->typeReturn->generatedrTypeLLVM(_contextGenCode->getBackend()->getContext());
            
            std::vector<llvm::Type*> paramTypes;
            
            // Ajouter le paramètre 'this' caché comme premier paramètre
            paramTypes.push_back(llvm::PointerType::getUnqual(_contextGenCode->getBackend()->getContext()));

            for (auto* arg : symbole->node->getArguments()) {
                auto* argFunction = prysma::cast<NodeArgFunction>(arg);
                paramTypes.push_back(argFunction->getType()->generatedrTypeLLVM(_contextGenCode->getBackend()->getContext()));
            }

            llvm::FunctionType* funcType = llvm::FunctionType::get(retType, paramTypes, false);
            
            // Mangling du nom pour LLVM (ex: Joueur_initialiser)
            std::string nomLLVM = nomClasse;
            nomLLVM += "_";
            nomLLVM += nomMethode;

            llvm::Function* vraieFunction = llvm::Function::Create(
                funcType, 
                llvm::Function::ExternalLinkage, 
                nomLLVM,          
                _contextGenCode->getBackend()->getModule()
            );
            
            symbole->function = vraieFunction;    
            
            // Construire la vtable pour cette classe
            std::vector<NodeDeclarationFunction*> listMethodeParent;
            if (classInfo->getParentHeritage() != nullptr) {
                MembersExtractorClass extracteurParent;
                classInfo->getParentHeritage()->accept(&extracteurParent);
                listMethodeParent = extracteurParent.getMethodes();
            }
            if(nomMethode != nomClasse) // Si la méthode n'est pas le builder, on le met dans la vtable, sinon on ne le met pas dans la vtable et on l'callle directement par son nom manglé
            {
                construireVTable(classInfo.get(), nomClasse, listMethodeParent);
            }
            // On vérifi si la classe contient un builder, sinon la classe n'est pas valide. 
            if (!classInfo->getRegistryFunctionLocale()->existe(nomClasse)) {
                throw std::runtime_error("Error : La classe '" + nomClasse + "' doit avoir un builder du même nom");
            }
        }
    }
}
