#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/Noeuds/Variable/NoeudDeclarationVariable.h"
#include "Compilateur/AST/Noeuds/Operande/NoeudTableauInitialisation.h"
#include "Compilateur/LLVM/GestionVariable.h"
#include "Compilateur/AST/Registre/Types/TypeTableau.h"
#include <llvm-18/llvm/IR/DerivedTypes.h>
#include <llvm/IR/Constants.h>

void VisiteurGeneralGenCode::visiter(NoeudDeclarationVariable* noeudDeclarationVariable) 
{
    GestionVariable gestionVariable(_contextGenCode);
    INoeud* expression = noeudDeclarationVariable->getExpression();
     // L'utilisation de dynamic_cast ici est un peu délicate, 
    // pour le moment je n'ai pas trouvé de solution optimal pour retirer ça, sinon je dois dépendre de llvm dans les INoeud, ce qui n'est pas idéal.
    // Le double dispatch est compliqué à mettre en place et ajouter une dépendance dans le context de génération du code 
    // Je dois trouver une solution pour éviter ce dynamic_cast, sans avoir c'est problème architectural. 
    // Sans doute utiliser un système de stratégie s'il y a plus de 2 types d'initialisation aussi. 
    // Beaucoup trop de static cast, je dois faire quelque chose pour éviter ça, c'est vraiment pas propre.
    
    // Vérifier si l'expression est une initialisation de tableau
    NoeudTableauInitialisation* tableauInit = dynamic_cast<NoeudTableauInitialisation*>(expression);
    
    if (tableauInit != nullptr) {
        // Déterminer le type LLVM du tableau
        llvm::Type* typeVariable = nullptr;
        llvm::Type* typeElement = nullptr;
        
        TypeTableau* typeTableauDecl = dynamic_cast<TypeTableau*>(noeudDeclarationVariable->getType());
        
        if (typeTableauDecl != nullptr && typeTableauDecl->getTaille() == nullptr) {
            // Taille implicite : calculer depuis l'initialisation
            size_t tailleReelle = tableauInit->getElements().size();
            typeElement = typeTableauDecl->getTypeEnfant()->genererTypeLLVM(_contextGenCode->backend->getContext());
            typeVariable = llvm::ArrayType::get(typeElement, tailleReelle);
        } else {
            // Taille explicite : utiliser le type déclaré
            typeVariable = noeudDeclarationVariable->getType()->genererTypeLLVM(_contextGenCode->backend->getContext());
            auto* typeTableauLLVM = llvm::dyn_cast<llvm::ArrayType>(typeVariable);
            if (typeTableauLLVM == nullptr) {
                throw std::runtime_error("Erreur : le type de la variable n'est pas un tableau pour une initialisation de tableau");
            }
            typeElement = typeTableauLLVM->getElementType();
        }
        
        // Allouer et enregistrer le tableau
        llvm::AllocaInst* allocaInstTableau = gestionVariable.allouerVariable(typeVariable, noeudDeclarationVariable->getNom());
        // Initialiser chaque élément du tableau
        auto* typeTableauLLVM = llvm::dyn_cast<llvm::ArrayType>(typeVariable);
        for (size_t i = 0; i < tableauInit->getElements().size(); ++i) {
            std::vector<llvm::Value*> indices = {
                _contextGenCode->backend->getBuilder().getInt32(0),
                _contextGenCode->backend->getBuilder().getInt32(static_cast<uint32_t>(i))
            }; 
            llvm::Value* ptrCase = _contextGenCode->backend->getBuilder().CreateGEP(typeTableauLLVM, allocaInstTableau, indices, "ptr_case");
            
            INoeud* element = tableauInit->getElements()[i];
            Symbole symboleElement = evaluerExpression(element);
            llvm::Value* valeurCastee = _contextGenCode->backend->creerAutoCast(symboleElement.adresse, typeElement);
            _contextGenCode->backend->getBuilder().CreateStore(valeurCastee, ptrCase);
        }
    } else {
        // Variable simple (non-tableau)
        llvm::Type* typeVariable = noeudDeclarationVariable->getType()->genererTypeLLVM(_contextGenCode->backend->getContext());
        llvm::AllocaInst* allocaInst = gestionVariable.allouerVariable(typeVariable, noeudDeclarationVariable->getNom());
        
        llvm::Value* valeurCalculee = evaluerExpression(expression).adresse;
        if (valeurCalculee == nullptr) {
            return;
        }

        llvm::Value* valeurCastee = _contextGenCode->backend->creerAutoCast(valeurCalculee, typeVariable);
        gestionVariable.stockerVariable(valeurCastee, allocaInst);
    }
}
