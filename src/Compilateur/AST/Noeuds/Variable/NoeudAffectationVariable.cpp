#include "Compilateur/AST/Noeuds/Variable/NoeudAffectationVariable.h"
#include "Compilateur/Visiteur/Interfaces/IVisiteur.h"
#include "Compilateur/Lexer/Lexer.h"

#include <llvm/IR/Instructions.h>
#include <llvm/IR/Value.h>
#include <memory>
#include <utility>

NoeudAffectationVariable::NoeudAffectationVariable(const std::string& nom, INoeud* expression, Token token)
    : _nom(nom),  _expression(expression), _token(std::move(token))
{

}

NoeudAffectationVariable::~NoeudAffectationVariable()
{
}

void NoeudAffectationVariable::accept(IVisiteur* visiteur)
{
    visiteur->visiter(this);
}
