#ifndef C281C518_FA13_4CFD_B295_5CB41352DB27
#define C281C518_FA13_4CFD_B295_5CB41352DB27

#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include <string>
#include "Compilateur/Visiteur/AmisVisiteurs.h"

class NoeudRefVariable : public INoeud
{
    LISTE_DES_AMIS_VISITEURS
private:
    std::string _nomVariable;

public:
    explicit NoeudRefVariable(const std::string& nomVariable);
    ~NoeudRefVariable();

    void accept(IVisiteur* visiteur) override;

};

#endif /* C281C518_FA13_4CFD_B295_5CB41352DB27 */
