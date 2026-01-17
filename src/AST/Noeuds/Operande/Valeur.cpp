#include "AST/Noeuds/Operande/Valeur.h"

Valeur::Valeur(double valeur) : _valeur(valeur) {}

double Valeur::resoudre() {
    return _valeur;
}
