#include "Compilateur/AST/Noeuds/Tableau/NoeudLectureTableau.h"


NoeudLectureTableau::NoeudLectureTableau(INoeud* indexEquation, Token nomTableau)
: _indexEquation(indexEquation), _nomTableau(std::move(nomTableau))
{}

NoeudLectureTableau::~NoeudLectureTableau()
{
}

void NoeudLectureTableau::accept(IVisiteur* visiteur)
{
   visiteur->visiter(this);
}
