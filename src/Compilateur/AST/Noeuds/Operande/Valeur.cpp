#include "Compilateur/AST/Noeuds/Operande/Valeur.h"
#include <stdexcept>

Valeur::Valeur(llvm::Value* valeur) : _valeur(valeur) {
    if (valeur == nullptr) {
        throw std::runtime_error("Erreur: Valeur créée avec un pointeur nullptr");
    }
}

void Valeur::accept(IVisiteur* visiteur)
{
   visiteur->visiter(this);
}

/*

llvm::Value* Valeur::genCode() {
    if (_valeur == nullptr) {
        throw std::runtime_error("Erreur: Valeur::genCode() retournerait nullptr");
    }
    return _valeur;
}
*/