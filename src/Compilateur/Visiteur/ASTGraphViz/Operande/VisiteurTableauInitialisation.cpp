#include "Compilateur/Visiteur/ASTGraphViz/VisiteurGeneralGraphViz.h"
#include "Compilateur/AST/Noeuds/Operande/NoeudTableauInitialisation.h"
#include <memory>

void VisiteurGeneralGraphViz::visiter(NoeudTableauInitialisation* noeud)
{
    int idNoeud = _sortieGrapheVisuel.ajouterNoeud("[...]");
    
    const std::vector<std::shared_ptr<INoeud>>&  elements = noeud->getElements();
    for (const std::shared_ptr<INoeud>& element : elements) {
        if (element) {
            element->accept(this);
            _sortieGrapheVisuel.ajouterArete(idNoeud, _dernierId);
        }
    }
    
    _dernierId = idNoeud;
}
