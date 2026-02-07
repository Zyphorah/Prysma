#include "Compilateur/AST/Noeuds/Fonction/NoeudArgPassFonction.h"
#include <llvm-18/llvm/IR/Value.h>

#include <utility>


NoeudArgPassFonction::NoeudArgPassFonction(std::shared_ptr<RegistreVariable> registreVariable, const Token& token, std::shared_ptr<RegistreArgument> registreArgument)
: _registreVariable(std::move(registreVariable)), _tokenVariable(token), _registreArgument(std::move(registreArgument))
{
}

NoeudArgPassFonction::~NoeudArgPassFonction()
{}

void NoeudArgPassFonction::accept(IVisiteur* visiteur)
{
    visiteur->visiter(this);
}

