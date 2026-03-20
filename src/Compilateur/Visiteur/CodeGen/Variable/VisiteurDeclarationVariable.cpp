#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include "Compilateur/AST/Registre/Types/IType.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/AST_Genere.h"
#include "Compilateur/LLVM/GestionVariable.h"
#include "Compilateur/AST/Registre/Types/TypeTableau.h"
#include <cstddef>
#include <cstdint>
#include <llvm-18/llvm/IR/DerivedTypes.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/Casting.h>
#include <vector>

#include "Compilateur/Visiteur/CodeGen/Helper/ErrorHelper.h"
#include "Compilateur/Utils/PrysmaCast.h"

void VisiteurGeneralGenCode::visiter(NoeudDeclarationVariable* noeudDeclarationVariable) 
{
    AllocateurVariable allocateur(_contextGenCode);
    INoeud* expression = noeudDeclarationVariable->getExpression();
       
    // Vérifier si l'expression est une initialisation de tableau
    auto* tableauInit = prysma::dyn_cast<NoeudTableauInitialisation>(expression);
    
    llvm::AllocaInst* allocaCree = nullptr; 
    
    if (tableauInit != nullptr) {
        // Déterminer le type LLVM du tableau
        llvm::Type* typeVariable = nullptr;
        llvm::Type* typeElement = nullptr;
        
        IType* typeDecl = noeudDeclarationVariable->getType();
        auto* typeTableauDecl = prysma::dyn_cast<TypeTableau>(typeDecl);
        
        if (typeTableauDecl != nullptr && typeTableauDecl->getTaille() == nullptr) {
            size_t tailleReelle = tableauInit->getElements().size();
            typeElement = typeTableauDecl->getTypeEnfant()->genererTypeLLVM(_contextGenCode->getBackend()->getContext());
            typeVariable = llvm::ArrayType::get(typeElement, tailleReelle);
        } else {
            typeVariable = noeudDeclarationVariable->getType()->genererTypeLLVM(_contextGenCode->getBackend()->getContext());
            if (typeVariable == nullptr) {
                ErrorHelper::erreurCompilation("Impossible de déterminer la taille du tableau");
            }
            auto* typeTableauLLVM = llvm::dyn_cast<llvm::ArrayType>(typeVariable);
            if (typeTableauLLVM == nullptr) {
                ErrorHelper::erreurCompilation("Le type de la variable n'est pas un tableau pour une initialisation de tableau");
            }
            typeElement = typeTableauLLVM->getElementType();
        }
        
        // Allouer et enregistrer le tableau
        llvm::AllocaInst* allocaInstTableau = allocateur.allouer(typeVariable, noeudDeclarationVariable->getNom());
        
        allocaCree = allocaInstTableau;

        // Initialiser chaque élément du tableau
        auto* typeTableauLLVM = llvm::dyn_cast<llvm::ArrayType>(typeVariable);
        for (size_t i = 0; i < tableauInit->getElements().size(); ++i) {
            std::vector<llvm::Value*> indices = {
                _contextGenCode->getBackend()->getBuilder().getInt32(0),
                _contextGenCode->getBackend()->getBuilder().getInt32(static_cast<uint32_t>(i))
            }; 
            llvm::Value* ptrCase = _contextGenCode->getBackend()->getBuilder().CreateGEP(typeTableauLLVM, allocaInstTableau, indices, "ptr_case");
            
            INoeud* element = tableauInit->getElements()[i];
            Symbole symboleElement = evaluerExpression(element);
            llvm::Value* valeurCastee = _contextGenCode->getBackend()->creerAutoCast(symboleElement.getAdresse(), typeElement);
            _contextGenCode->getBackend()->getBuilder().CreateStore(valeurCastee, ptrCase);
        }
    } else {
        // Variable simple (non-tableau)
        llvm::Type* typeVariable = noeudDeclarationVariable->getType()->genererTypeLLVM(_contextGenCode->getBackend()->getContext());
        llvm::AllocaInst* allocaInst = allocateur.allouer(typeVariable, noeudDeclarationVariable->getNom());
        allocaCree = allocaInst;
        
        llvm::Value* valeurCalculee = evaluerExpression(expression).getAdresse();
        if (valeurCalculee == nullptr) {
            return;
        }

        llvm::Value* valeurCastee = _contextGenCode->getBackend()->creerAutoCast(valeurCalculee, typeVariable);
        allocateur.stocker(valeurCastee, allocaInst);
    }
    if (allocaCree != nullptr) {
        Token token;
        token.value = noeudDeclarationVariable->getNom();
        
        // Créer le symbole avec le type pointé si c'est un pointeur
        llvm::Type* typeVariable = noeudDeclarationVariable->getType()->genererTypeLLVM(_contextGenCode->getBackend()->getContext());
        
        // Si on a un tableau avec taille dynamique (nullptr), on doit utiliser le type réel qu'on a calculé
        if (typeVariable == nullptr) {
            IType* typeDecl = noeudDeclarationVariable->getType();
            auto* typeTableauDecl = prysma::dyn_cast<TypeTableau>(typeDecl);
            if (typeTableauDecl != nullptr && tableauInit != nullptr) {
                // On peut recalculer à partir de l'initialisation
                size_t tailleReelle = tableauInit->getElements().size();
                llvm::Type* typeElement = typeTableauDecl->getTypeEnfant()->genererTypeLLVM(_contextGenCode->getBackend()->getContext());
                typeVariable = llvm::ArrayType::get(typeElement, tailleReelle);
            } else {
                ErrorHelper::erreurCompilation("Impossible de déterminer le type de la variable");
            }
        }
        
        llvm::Type* typePointeElement = nullptr;
        
        if (typeVariable->isPointerTy()) {
            // Pour un pointeur, on récupère le type pointé qui a été mémorisé lors de l'évaluation de l'expression (new)
            typePointeElement = _contextGenCode->getValeurTemporaire().getTypePointeElement();
        }
        
        _contextGenCode->getRegistreVariable()->enregistrer(
            token, 
            Symbole(allocaCree, noeudDeclarationVariable->getType(), typePointeElement)
        );
    }
}