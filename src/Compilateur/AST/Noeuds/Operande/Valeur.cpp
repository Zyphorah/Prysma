#include "AST/Noeuds/Operande/Valeur.h"

Valeur::Valeur(llvm::Value* valeur) : _valeur(valeur) {}

llvm::Value* Valeur::resoudre() {
    return _valeur;
}
