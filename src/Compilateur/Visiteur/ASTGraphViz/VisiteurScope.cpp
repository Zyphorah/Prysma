#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Noeuds/NoeudScope.h"
#include "Compilateur/Visiteur/ASTGraphViz/VisiteurGeneralGraphViz.h"
#include <memory>

class NoeudScope;

void VisiteurGeneralGraphViz::visiter(NoeudScope* noeudScope)
{
    int idNoeud = _sortieGrapheVisuel.ajouterNoeud("Scope");

    for (INoeud* enfant : noeudScope->getEnfants()) {
        enfant->accept(this);
        _sortieGrapheVisuel.ajouterArete(idNoeud, _dernierId);
    }
    _dernierId = idNoeud;
}