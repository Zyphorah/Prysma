#include "Compilateur/AST/Noeuds/Tableau/NoeudLectureTableau.h"


NoeudLectureTableau::NoeudLectureTableau(std::shared_ptr<INoeud> indexEquation, std::string nomTableau)
: _indexEquation(std::move(indexEquation)), _nomTableau(std::move(nomTableau))
{}

NoeudLectureTableau::~NoeudLectureTableau()
{
}

void NoeudLectureTableau::accept(IVisiteur* visiteur)
{
   visiteur->visiter(this);
}
