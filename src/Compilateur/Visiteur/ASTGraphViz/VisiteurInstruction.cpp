#include "Compilateur/Visiteur/ASTGraphViz/VisiteurGeneralGraphViz.h"
#include "Compilateur/AST/Noeuds/NoeudInstruction.h"
#include <memory>

void VisiteurGeneralGraphViz::visiter(NoeudInstruction* instruction)
{
    int idNoeud = _sortieGrapheVisuel.ajouterNoeud("Instruction");

    for (INoeud* enfant : instruction->getEnfants()) {
        enfant->accept(this);
        _sortieGrapheVisuel.ajouterArete(idNoeud, _dernierId);
    }
    _dernierId = idNoeud;
}
