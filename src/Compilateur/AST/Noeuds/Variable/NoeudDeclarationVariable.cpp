#include "Compilateur/AST/Noeuds/Variable/NoeudDeclarationVariable.h"
#include "Compilateur/Visiteur/Interfaces/IVisiteur.h"

#include <llvm/IR/Instructions.h>
#include <llvm/IR/Value.h>
#include <memory>
#include <utility>

NoeudDeclarationVariable::NoeudDeclarationVariable(const std::string& nom, IType* type, INoeud* expression)
    : _nom(nom), _type(type), _expression(expression)
{
}

NoeudDeclarationVariable::~NoeudDeclarationVariable()
{
}

void NoeudDeclarationVariable::accept(IVisiteur* visiteur)
{
    visiteur->visiter(this);
}
