#ifndef E05157BF_22F2_4BDD_9434_2D8D64770D9D
#define E05157BF_22F2_4BDD_9434_2D8D64770D9D
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/Visiteur/Interfaces/IVisiteur.h"
#include <memory>
#include <string>

// dec int a = tableau[0];
class NoeudLectureTableau : public INoeud
{
private:
     std::shared_ptr<INoeud> _indexEquation;
     std::string _nomTableau; 
public: 
     NoeudLectureTableau(std::shared_ptr<INoeud> indexEquation, std::string nomTableau);
    ~NoeudLectureTableau();

     std::shared_ptr<INoeud> getIndexEquation(){return _indexEquation;};
     std::string getNomTableau(){return _nomTableau;};
     
     void accept(IVisiteur* visiteur) override;
};

#endif /* E05157BF_22F2_4BDD_9434_2D8D64770D9D */
