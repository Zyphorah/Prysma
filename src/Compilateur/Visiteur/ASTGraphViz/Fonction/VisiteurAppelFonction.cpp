#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/LLVM/GenerateurFonction.h"
#include "Compilateur/Visiteur/ASTGraphViz/VisiteurGeneralGraphViz.h"
#include "Compilateur/AST/Noeuds/Fonction/NoeudAppelFonction.h"
#include <memory>

void VisiteurGeneralGraphViz::visiter(NoeudAppelFonction* noeudAppelFonction)
{
    int idNoeud = _sortieGrapheVisuel.ajouterNoeud("Appel Fonction: " + noeudAppelFonction->getNomFonction().value);

    for (INoeud* enfant : noeudAppelFonction->getEnfants()) {
        enfant->accept(this);
        _sortieGrapheVisuel.ajouterArete(idNoeud, _dernierId);
    }
    _dernierId = idNoeud;
}