#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/Noeuds/Variable/NoeudDeclarationVariable.h"
#include "Compilateur/LLVM/GestionVariable.h"

void VisiteurGeneralGenCode::visiter(NoeudDeclarationVariable* noeudDeclarationVariable) 
{
    GestionVariable gestionVariable(_contextGenCode);

    // Allouer de la mémoire pour la variable
    llvm::Type* typeVariable = _contextGenCode->registreType->recuperer(noeudDeclarationVariable->_tokenType);
    llvm::AllocaInst* allocaInst = gestionVariable.allouerVariable(typeVariable, noeudDeclarationVariable->_nom);

    // Enregistrer la variable
    gestionVariable.enregistrerVariable(noeudDeclarationVariable->_nom, allocaInst);

    // Évaluer l'expression d'initialisation
    llvm::Value* valeurCalculee = evaluerExpression(noeudDeclarationVariable->_expression);

    // Stocker la valeur
    llvm::Value* valeurCastee = _contextGenCode->backend->creerAutoCast(valeurCalculee, typeVariable);
    gestionVariable.stockerVariable(valeurCastee, allocaInst);
}

