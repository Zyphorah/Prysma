#include "Compilateur/AST/Noeuds/NoeudInstruction.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/Visiteur/Interfaces/IVisiteur.h"
#include "Compilateur/AST/AST_Genere.h"

NoeudInstruction::NoeudInstruction()
= default;

void NoeudInstruction::accept(IVisiteur* visiteur)
{
    visiteur->visiter(this);
}

void NoeudInstruction::ajouterInstruction(INoeud* enfant)
{
    if (enfant != nullptr) {
        enfants.push_back(enfant);
    }
}

NoeudTypeGenere NoeudInstruction::getTypeGenere() const
{
    return NoeudTypeGenere::AppelFonction; 
}
