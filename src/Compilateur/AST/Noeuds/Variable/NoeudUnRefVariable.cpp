#include "Compilateur/AST/Noeuds/Variable/NoeudUnRefVariable.h"

NoeudUnRefVariable::NoeudUnRefVariable(const std::string& nomVariable)
    : _nomVariable(nomVariable)
{
}

NoeudUnRefVariable::~NoeudUnRefVariable()
{
}

void NoeudUnRefVariable::accept(IVisiteur* visiteur)
{
    visiteur->visiter(this);
}
