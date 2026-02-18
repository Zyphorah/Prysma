#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/Noeuds/Variable/NoeudDeclarationVariable.h"
#include "Compilateur/AST/Noeuds/Operande/NoeudTableauInitialisation.h"
#include "Compilateur/LLVM/GestionVariable.h"
#include <llvm-18/llvm/IR/DerivedTypes.h>
#include <llvm/IR/Constants.h>
#include <memory>

void VisiteurGeneralGenCode::visiter(NoeudDeclarationVariable* noeudDeclarationVariable) 
{
    GestionVariable gestionVariable(_contextGenCode);

    // Générer le type LLVM via la hiérarchie IType
    llvm::Type* typeVariable = noeudDeclarationVariable->getType()->genererTypeLLVM(_contextGenCode->backend->getContext());
   

    // Vérifier si l'expression est une initialisation de tableau
    std::shared_ptr<INoeud> expression = noeudDeclarationVariable->getExpression();
    
    // L'utilisation de dynamic_cast ici est un peu délicate, 
    // pour le moment je n'ai pas trouvé de solution optimal pour retirer ça, sinon je dois dépendre de llvm dans les INoeud, ce qui n'est pas idéal.
    // Le double dispatch est compliqué à mettre en place et ajouter une dépendance dans le context de génération du code 
    // Je dois trouver une solution pour éviter ce dynamic_cast, sans avoir c'est problème architectural. 
    // Sans doute utiliser un système de stratégie s'il y a plus de 2 types d'initialisation aussi. 

    if (NoeudTableauInitialisation* tableauInit = dynamic_cast<NoeudTableauInitialisation*>(expression.get())) {
        // typeVariable est le type du tableau complet [n x T], on extrait T
        llvm::ArrayType* typeTableau = llvm::dyn_cast<llvm::ArrayType>(typeVariable);
        
        if (typeTableau == nullptr) {
            throw std::runtime_error("Erreur : le type de la variable n'est pas un tableau pour une initialisation de tableau");
        }
        
        llvm::Type* typeElement = typeTableau->getElementType();
        
        // Allouer le tableau
        llvm::AllocaInst* allocaInstTableau = gestionVariable.allouerVariable(typeVariable, noeudDeclarationVariable->getNom());
        gestionVariable.enregistrerVariable(noeudDeclarationVariable->getNom(), allocaInstTableau);
        
        // Boucle d'initialisation GEP et store
        for (size_t i = 0; i < tableauInit->getElements().size(); ++i) {
            std::vector<llvm::Value*> indices = {
                _contextGenCode->backend->getBuilder().getInt32(0), // Toujours à zéro c'est l'index d'offset pour les tableaux alloués 
                _contextGenCode->backend->getBuilder().getInt32(i)  
            }; 
            llvm::Value* ptrCase = _contextGenCode->backend->getBuilder().CreateGEP(typeTableau, allocaInstTableau, indices, "ptr_case");
            
            // Évaluer et stocker la valeur de l'élément
            std::shared_ptr<INoeud> element = tableauInit->getElements()[i];
            llvm::Value* valeurElement = evaluerExpression(element);
            llvm::Value* valeurCastee = _contextGenCode->backend->creerAutoCast(valeurElement, typeElement);
            _contextGenCode->backend->getBuilder().CreateStore(valeurCastee, ptrCase);
        }     
    } else {
        llvm::AllocaInst* allocaInst = gestionVariable.allouerVariable(typeVariable, noeudDeclarationVariable->getNom());
        // Enregistrer la variable
        gestionVariable.enregistrerVariable(noeudDeclarationVariable->getNom(), allocaInst);
        // Cas normal: évaluer l'expression d'initialisation
        llvm::Value* valeurCalculee = evaluerExpression(noeudDeclarationVariable->getExpression());
        if (valeurCalculee == nullptr) {
            return;
        }

        // Stocker la valeur
        llvm::Value* valeurCastee = _contextGenCode->backend->creerAutoCast(valeurCalculee, typeVariable);
        gestionVariable.stockerVariable(valeurCastee, allocaInst);
    }
}
