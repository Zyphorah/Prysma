#include "Compilateur/AST/Noeuds/Fonction/NoeudDeclarationFonction.h"
#include "Compilateur/Visiteur/Interfaces/IVisiteur.h"
#include <llvm-18/llvm/IR/BasicBlock.h>
#include <llvm-18/llvm/IR/DerivedTypes.h>
#include <llvm-18/llvm/IR/Function.h>
#include <memory>
#include <utility>

NoeudDeclarationFonction::NoeudDeclarationFonction(std::string nom, IType* typeRetour)
    : _nom(std::move(nom)), _typeRetour(typeRetour)
{
}

void NoeudDeclarationFonction::accept(IVisiteur* visiteur)
{
    visiteur->visiter(this);
}
