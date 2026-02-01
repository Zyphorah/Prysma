#include "Compilateur/AST/Noeuds/Operande/NoeudVariable.h"
#include <stdexcept>

NoeudVariable::NoeudVariable(llvm::Value* adresseMemoire, const std::string& nomVariable, std::shared_ptr<GestionnaireChargementVariable> gestionnaire)
    : _adresseMemoire(adresseMemoire), _nomVariable(nomVariable), _gestionnaire(gestionnaire)
{
    if (adresseMemoire == nullptr) {
        throw std::runtime_error("Erreur: NoeudVariable créé avec une adresse mémoire nullptr");
    }
    if (gestionnaire == nullptr) {
        throw std::runtime_error("Erreur: NoeudVariable créé sans gestionnaire de chargement");
    }
}

NoeudVariable::~NoeudVariable()
{
}

llvm::Value* NoeudVariable::genCode()
{
    return _gestionnaire->chargerVariable(_adresseMemoire, _nomVariable);
}
