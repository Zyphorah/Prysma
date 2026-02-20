#include "Compilateur/LLVM/GenerateurFonction.h"
#include "Compilateur/Visiteur/ASTGraphViz/VisiteurGeneralGraphViz.h"
#include "Compilateur/AST/Noeuds/Fonction/NoeudDeclarationFonction.h"

#include <memory>

void VisiteurGeneralGraphViz::visiter(NoeudDeclarationFonction* noeudDeclarationFonction) 
{
    int idNoeud = _sortieGrapheVisuel.ajouterNoeud("Declaration Fonction: " + noeudDeclarationFonction->getNom());

    for (INoeud* enfant : noeudDeclarationFonction->getEnfants()) {
        enfant->accept(this);
        _sortieGrapheVisuel.ajouterArete(idNoeud, _dernierId);
    }
    _dernierId = idNoeud;
}