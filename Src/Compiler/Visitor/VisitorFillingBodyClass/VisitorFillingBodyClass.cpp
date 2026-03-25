#include "Compiler/Visitor/VisitorFillingBodyClass/VisitorFillingBodyClass.h"
#include "Compiler/AST/Registry/ContextGenCode.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Registry/RegistryClass.h"
#include "Compiler/AST/Registry/RegistryFunction.h"
#include "Compiler/Visitor/VisitorBaseGenerale.h"
#include "Compiler/Visitor/Extractors/MembersExtractorClass.h"
#include "Compiler/Utils/PrysmaCast.h"
#include <llvm-18/llvm/IR/Function.h>
#include <llvm-18/llvm/IR/GlobalVariable.h>
#include <llvm-18/llvm/IR/Metadata.h>
#include <llvm/IR/Constant.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/GlobalValue.h>
#include <stdexcept>
#include <string>
#include <vector>

FillingVisitorCoprsClass::FillingVisitorCoprsClass(ContextGenCode* contextGenCode)
    : _contextGenCode(contextGenCode)
{
}

FillingVisitorCoprsClass::~FillingVisitorCoprsClass()
= default;


void FillingVisitorCoprsClass::visiter(NodeClass* nodeClass)
{
    std::string nomClass = nodeClass->getNomClass().value;

    MembersExtractorClass extracteurClass;
    for (INode* membre : nodeClass->getListMembers()) {
        if (membre != nullptr) {
            membre->accept(&extracteurClass);
        }
    }

    // 1. Récupération de l'object class 
    Class* classInfo = _contextGenCode->getRegistryClass()->recuperer(nomClass).get();

    // 2. Résolution récursive de l'héritage 
    INode* ParentHeritage = classInfo->getParentHeritage();
    if(ParentHeritage != nullptr)
    {
        ParentHeritage->accept(this);
    }

    // 3. Initialization du layout ( vecteur de type) 
    std::vector<llvm::Type*> elementsBodyClass;

    // Faire le vpointeur à l'adresse zéro 
    elementsBodyClass.push_back(llvm::PointerType::get(
        llvm::Type::getInt8Ty(
        _contextGenCode->getBackend()->getContext()),
         0
        ));


    // 4. Héritage des champs du parent et construction de la VTable
    if(ParentHeritage != nullptr)
    {
        MembersExtractorClass extracteurParent;
        ParentHeritage->accept(&extracteurParent);
        std::string nomParent = extracteurParent.getNomClass();

        Class* classParentInfo = _contextGenCode->getRegistryClass()->recuperer(nomParent).get();

        // On extrait les méthodes du parent pour les binder à la même position de la vtable
        auto clesMethodesParent = classParentInfo->getRegistryFunctionLocale()->obtenirCles();
      
        std::vector<NodeDeclarationFunction*> listMethodeParent;
          for (const auto& cle : clesMethodesParent) {
              const auto& symbole = classParentInfo->getRegistryFunctionLocale()->recuperer(cle);
              if (!prysma::isa<SymboleFunctionLocale>(symbole.get())) {
                  throw std::runtime_error("Error : SymboleFunctionLocale attendu");
              }
              auto* symboleLocal = prysma::cast<SymboleFunctionLocale>(symbole.get());
              if (symboleLocal->node != nullptr) {
                MembersExtractorClass methodeExtr;
                symboleLocal->node->accept(&methodeExtr);
                if (!methodeExtr.getMethodes().empty()) {
                    listMethodeParent.push_back(methodeExtr.getMethodes()[0]);
                }
            }
        }

        // On regarde si il y a les méthodes du parent dans la classe courante, si non c'est une error 
        for(NodeDeclarationFunction* methodeParent : listMethodeParent)
        {
            bool trouve = false;
            for(NodeDeclarationFunction* methodeClasse : extracteurClass.getMethodes())
            {
                if(methodeClasse->getNom() == methodeParent->getNom())
                {
                    trouve = true;
                    break;
                }
            }

            if(!trouve)
            {
                std::string errorMsg = "La classe " + nomClass + " doit implémenter la méthode ";
                errorMsg += methodeParent->getNom();
                errorMsg += " héritée de " + nomParent;
                throw std::runtime_error(errorMsg);
            }
        }

    }
   
    // 5. Extraction des variables de l'AST
    // L'index commence à 1 car l'index 0 est réservé au vtable pointer
    unsigned int indexCourant = 1;
    
    // Parcourir les membres pour récupérer les variables dans l'ordre de déclaration
    for(NodeDeclarationVariable* declarationVariable : extracteurClass.getVariables())
    {
        IType* itype = declarationVariable->getType();
        llvm::Type* typeVariable = itype->generatedrTypeLLVM(_contextGenCode->getBackend()->getContext());
        if (typeVariable != nullptr) {
            elementsBodyClass.push_back(typeVariable);
            // Enregistryr l'index pour la Passe 3
            classInfo->getMemberIndices()[declarationVariable->getNom()] = indexCourant;
            indexCourant++;
        }
    }

    // 6. Scellage de la structure (setBody)
    if (classInfo->getStructType() != nullptr) {
        classInfo->getStructType()->setBody(elementsBodyClass);
    }
}