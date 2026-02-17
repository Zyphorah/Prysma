#include "Compilateur/Visiteur/ASTGraphViz/VisiteurGeneralGraphViz.h"
#include "Compilateur/AST/Noeuds/Variable/NoeudAffectationTableau.h"

void VisiteurGeneralGraphViz::visiter(NoeudAffectationTableau* noeudAffectationTableau)
{
    int idNoeud = _sortieGrapheVisuel.ajouterNoeud("Affectation Tableau: " + noeudAffectationTableau->getNom());

    // Visiter l'expression de l'index
    noeudAffectationTableau->getExpressionIndex()->accept(this);
    _sortieGrapheVisuel.ajouterArete(idNoeud, _dernierId);
    
    // Visiter l'expression à affecter
    noeudAffectationTableau->getExpression()->accept(this);
    _sortieGrapheVisuel.ajouterArete(idNoeud, _dernierId);
    
    _dernierId = idNoeud;
}
