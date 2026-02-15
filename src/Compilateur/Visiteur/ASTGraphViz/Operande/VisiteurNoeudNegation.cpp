#include "Compilateur/Visiteur/ASTGraphViz/VisiteurGeneralGraphViz.h"
#include "Compilateur/AST/Noeuds/Operande/NoeudNegation.h"
#include <memory>

void VisiteurGeneralGraphViz::visiter(NoeudNegation* noeud)
{
    int idNoeud = _sortieGrapheVisuel.ajouterNoeud("!");
    
    const std::shared_ptr<INoeud>& operande = noeud->getOperande();
    if (operande) {
        operande->accept(this);
        _sortieGrapheVisuel.ajouterArete(idNoeud, _dernierId);
    }
    
    _dernierId = idNoeud;
}
