#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/Visiteur/ASTGraphViz/VisiteurGeneralGraphViz.h"
#include "Compilateur/AST/Noeuds/Tableau/NoeudLectureTableau.h"
#include <memory>

void VisiteurGeneralGraphViz::visiter(NoeudLectureTableau* noeudLectureTableau)
{
    std::string nomTableau = noeudLectureTableau->getNomTableau();
    std::shared_ptr<INoeud> indexEquation = noeudLectureTableau->getIndexEquation();

    int idNoeud = _sortieGrapheVisuel.ajouterNoeud("Lecture: " + nomTableau);

    if (indexEquation != nullptr) {
        indexEquation->accept(this);
        _sortieGrapheVisuel.ajouterArete(idNoeud, _dernierId);
    }

    _dernierId = idNoeud;
}


