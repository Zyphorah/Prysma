#ifndef E05157BF_22F2_4BDD_9434_2D8D64770D9D
#define E05157BF_22F2_4BDD_9434_2D8D64770D9D
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/Visiteur/Interfaces/IVisiteur.h"

// dec int a = tableau[0];
class NoeudLectureTableau : public INoeud
{
private:
     INoeud* _indexEquation;
     Token _nomTableau; 
public: 
     NoeudLectureTableau(INoeud* indexEquation, Token nomTableau);
    ~NoeudLectureTableau();

     INoeud* getIndexEquation(){return _indexEquation;};
     Token getNomTableau(){return _nomTableau;};
     void accept(IVisiteur* visiteur) override;
};

#endif /* E05157BF_22F2_4BDD_9434_2D8D64770D9D */
