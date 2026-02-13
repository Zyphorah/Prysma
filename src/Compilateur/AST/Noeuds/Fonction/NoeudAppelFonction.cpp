#include "Compilateur/AST/Noeuds/Fonction/NoeudAppelFonction.h"

NoeudAppelFonction::NoeudAppelFonction(Token nomFonction) : _nomFonction(std::move(nomFonction))
{
    
}

NoeudAppelFonction::~NoeudAppelFonction()
{}

void NoeudAppelFonction::accept(IVisiteur* visiteur)
{
    visiteur->visiter(this);
}
