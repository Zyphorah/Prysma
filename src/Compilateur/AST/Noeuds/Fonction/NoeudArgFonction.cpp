#include "Compilateur/AST/Noeuds/Fonction/NoeudArgFonction.h"
#include "Compilateur/AST/Registre/Types/IType.h"
#include "Compilateur/Visiteur/Interfaces/IVisiteur.h"

NoeudArgFonction::NoeudArgFonction(IType* type, std::string nom)
{
    _type = type;
    _nom = std::move(nom);
}

NoeudArgFonction::~NoeudArgFonction()
{
}

void NoeudArgFonction::accept(IVisiteur* visiteur)
{
    visiteur->visiter(this);
}

