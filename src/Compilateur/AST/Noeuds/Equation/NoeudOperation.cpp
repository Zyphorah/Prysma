#include "Compilateur/AST/Noeuds/Equation/NoeudOperation.h"
#include "Compilateur/Visiteur/Interfaces/IVisiteur.h"

NoeudOperation::NoeudOperation(TokenType type) 
    : typeOperation(type), gauche(nullptr), droite(nullptr) {
}

void NoeudOperation::accept(IVisiteur* visiteur) {
    if (visiteur != nullptr) {
        visiteur->visiter(this);
    }
}
