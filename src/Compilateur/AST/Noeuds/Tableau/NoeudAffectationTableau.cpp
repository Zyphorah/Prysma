#include "Compilateur/AST/Noeuds/Tableau/NoeudAffectationTableau.h"
#include "Compilateur/Visiteur/Interfaces/IVisiteur.h"
#include "Compilateur/Lexer/Lexer.h"

#include <llvm/IR/Instructions.h>
#include <llvm/IR/Value.h>
#include <memory>
#include <utility>

NoeudAffectationTableau::NoeudAffectationTableau(const std::string& nom, std::shared_ptr<INoeud> expressionIndex, std::shared_ptr<INoeud> expression, Token token)
    : _nom(nom), _expressionIndex(std::move(expressionIndex)), _expression(std::move(expression)), _token(std::move(token))
{
}

NoeudAffectationTableau::~NoeudAffectationTableau()
{
}

void NoeudAffectationTableau::accept(IVisiteur* visiteur)
{
    visiteur->visiter(this);
}
