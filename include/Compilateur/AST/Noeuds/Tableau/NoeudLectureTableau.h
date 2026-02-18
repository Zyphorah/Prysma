#ifndef E05157BF_22F2_4BDD_9434_2D8D64770D9D
#define E05157BF_22F2_4BDD_9434_2D8D64770D9D
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/Lexer/Lexer.h"
#include <memory>

// dec int a = tableau[0];
class NoeudLectureTableau : public INoeud
{
private:
     std::shared_ptr<INoeud> indexEquation;
     std::string token; 
public: 
     NoeudLectureTableau();
    ~NoeudLectureTableau();
     void accept(IVisiteur* visiteur) override;
};

#endif /* E05157BF_22F2_4BDD_9434_2D8D64770D9D */
