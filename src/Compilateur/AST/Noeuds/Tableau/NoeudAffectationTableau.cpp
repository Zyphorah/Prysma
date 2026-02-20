#include "Compilateur/AST/Noeuds/Tableau/NoeudAffectationTableau.h"
#include "Compilateur/Visiteur/Interfaces/IVisiteur.h"
#include "Compilateur/Lexer/Lexer.h"

#include <llvm/IR/Instructions.h>
#include <llvm/IR/Value.h>
#include <memory>
#include <utility>

NoeudAffectationTableau::NoeudAffectationTableau(const std::string& nom, INoeud* expressionIndex, INoeud* expression, Token token)
    : _nom(nom), _expressionIndex(expressionIndex), _expression(expression), _token(std::move(token))
{
}

NoeudAffectationTableau::~NoeudAffectationTableau()
{
}

void NoeudAffectationTableau::accept(IVisiteur* visiteur)
{
    visiteur->visiter(this);
}
