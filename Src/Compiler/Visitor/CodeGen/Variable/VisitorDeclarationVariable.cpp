#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/Stack/RegistryVariable.h"
#include "Compiler/AST/Registry/Types/IType.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Visitor/CodeGen/VisitorGeneralGenCode.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/LLVM/GestionVariable.h"
#include "Compiler/AST/Registry/Types/TypeArray.h"
#include <cstddef>
#include <cstdint>
#include <llvm-18/llvm/IR/DerivedTypes.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Support/Casting.h>
#include <vector>

#include "Compiler/Visitor/CodeGen/Helper/ErrorHelper.h"
#include "Compiler/Utils/PrysmaCast.h"

void GeneralVisitorGenCode::visiter(NodeDeclarationVariable* nodeDeclarationVariable) 
{
    AllocateurVariable allocateur(_contextGenCode);
    INode* expression = nodeDeclarationVariable->getExpression();
       
    // Vérifier si l'expression est une initialization de array
    auto* arrayInit = prysma::dyn_cast<NodeArrayInitialization>(expression);
    
    llvm::AllocaInst* allocaCree = nullptr; 
    
    if (arrayInit != nullptr) {
        // Déterminer le type LLVM du array
        llvm::Type* typeVariable = nullptr;
        llvm::Type* typeElement = nullptr;
        
        IType* typeDecl = nodeDeclarationVariable->getType();
        auto* typeArrayDecl = prysma::dyn_cast<TypeArray>(typeDecl);
        
        if (typeArrayDecl != nullptr && typeArrayDecl->getTaille() == nullptr) {
            size_t tailleReelle = arrayInit->getElements().size();
            typeElement = typeArrayDecl->getTypeChild()->generatedrTypeLLVM(_contextGenCode->getBackend()->getContext());
            typeVariable = llvm::ArrayType::get(typeElement, tailleReelle);
        } else {
            typeVariable = nodeDeclarationVariable->getType()->generatedrTypeLLVM(_contextGenCode->getBackend()->getContext());
            if (typeVariable == nullptr) {
                ErrorHelper::errorCompilation("Impossible de déterminer la taille du array");
            }
            auto* typeArrayLLVM = llvm::dyn_cast<llvm::ArrayType>(typeVariable);
            if (typeArrayLLVM == nullptr) {
                ErrorHelper::errorCompilation("Le type de la variable n'est pas un array pour une initialization de array");
            }
            typeElement = typeArrayLLVM->getElementType();
        }
        
        // Allouer et enregistryr le array
        llvm::AllocaInst* allocaInstArray = allocateur.allouer(typeVariable, nodeDeclarationVariable->getNom());
        
        allocaCree = allocaInstArray;

        // Initialiser chaque élément du array
        auto* typeArrayLLVM = llvm::dyn_cast<llvm::ArrayType>(typeVariable);
        for (size_t i = 0; i < arrayInit->getElements().size(); ++i) {
            std::vector<llvm::Value*> indices = {
                _contextGenCode->getBackend()->getBuilder().getInt32(0),
                _contextGenCode->getBackend()->getBuilder().getInt32(static_cast<uint32_t>(i))
            }; 
            llvm::Value* ptrCase = _contextGenCode->getBackend()->getBuilder().CreateGEP(typeArrayLLVM, allocaInstArray, indices, "ptr_case");
            
            INode* element = arrayInit->getElements()[i];
            Symbole symboleElement = evaluerExpression(element);
            llvm::Value* valeurCastee = _contextGenCode->getBackend()->creerAutoCast(symboleElement.getAdresse(), typeElement);
            _contextGenCode->getBackend()->getBuilder().CreateStore(valeurCastee, ptrCase);
        }
    } else {
        // Variable simple (non-array)
        llvm::Type* typeVariable = nodeDeclarationVariable->getType()->generatedrTypeLLVM(_contextGenCode->getBackend()->getContext());
        llvm::AllocaInst* allocaInst = allocateur.allouer(typeVariable, nodeDeclarationVariable->getNom());
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
        token.value = nodeDeclarationVariable->getNom();
        
        // Créer le symbole avec le type pointé si c'est un pointeur
        llvm::Type* typeVariable = nodeDeclarationVariable->getType()->generatedrTypeLLVM(_contextGenCode->getBackend()->getContext());
        
        // Si on a un array avec taille dynamique (nullptr), on doit utiliser le type réel qu'on a calculé
        if (typeVariable == nullptr) {
            IType* typeDecl = nodeDeclarationVariable->getType();
            auto* typeArrayDecl = prysma::dyn_cast<TypeArray>(typeDecl);
            if (typeArrayDecl != nullptr && arrayInit != nullptr) {
                // On peut recalculer à partir de l'initialization
                size_t tailleReelle = arrayInit->getElements().size();
                llvm::Type* typeElement = typeArrayDecl->getTypeChild()->generatedrTypeLLVM(_contextGenCode->getBackend()->getContext());
                typeVariable = llvm::ArrayType::get(typeElement, tailleReelle);
            } else {
                ErrorHelper::errorCompilation("Impossible de déterminer le type de la variable");
            }
        }
        
        llvm::Type* typePointeElement = nullptr;
        
        if (typeVariable->isPointerTy()) {
            // Pour un pointeur, on récupère le type pointé qui a été mémorisé lors de l'évaluation de l'expression (new)
            typePointeElement = _contextGenCode->getValeurTemporaire().getTypePointeElement();
        }
        
        _contextGenCode->getRegistryVariable()->enregistryr(
            token, 
            Symbole(allocaCree, nodeDeclarationVariable->getType(), typePointeElement)
        );
    }
}