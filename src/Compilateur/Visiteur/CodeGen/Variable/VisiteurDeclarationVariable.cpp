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
    llvm::AllocaInst* allocaInst = gestionVariable.allouerVariable(typeVariable, noeudDeclarationVariable->getNom());

    // Enregistrer la variable
    gestionVariable.enregistrerVariable(noeudDeclarationVariable->getNom(), allocaInst);

    // Vérifier si l'expression est une initialisation de tableau
    std::shared_ptr<INoeud> expression = noeudDeclarationVariable->getExpression();
    if (NoeudTableauInitialisation* tableauInit = dynamic_cast<NoeudTableauInitialisation*>(expression.get())) {
        // Initialiser le tableau avec les valeurs fournies
        const vector<std::shared_ptr<INoeud>>& elements = tableauInit->getElements();
        std::vector<llvm::Constant*> arrayElements;
        
        for (const std::shared_ptr<INoeud>& element : elements) {
            // Évaluer chaque élément
            element->accept(this);
            llvm::Value* valeur = _contextGenCode->valeurTemporaire;
            
            if (llvm::Constant* constantValue = llvm::dyn_cast<llvm::Constant>(valeur)) {
                arrayElements.push_back(constantValue);
            } else {
                // Si ce n'est pas une constante, créer une constante zéro
                arrayElements.push_back(llvm::Constant::getNullValue(valeur->getType()));
            }
        }
        
        // Créer un constant array
        llvm::ArrayType* arrayType = llvm::dyn_cast<llvm::ArrayType>(typeVariable);
        if (arrayType != nullptr) {
            llvm::Constant* arrayConstant = llvm::ConstantArray::get(arrayType, arrayElements);
            gestionVariable.stockerVariable(arrayConstant, allocaInst);
        }
    } else {
        // Cas normal: évaluer l'expression d'initialisation
        llvm::Value* valeurCalculee = evaluerExpression(noeudDeclarationVariable->getExpression());

        // Stocker la valeur
        llvm::Value* valeurCastee = _contextGenCode->backend->creerAutoCast(valeurCalculee, typeVariable);
        gestionVariable.stockerVariable(valeurCastee, allocaInst);
    }
}
