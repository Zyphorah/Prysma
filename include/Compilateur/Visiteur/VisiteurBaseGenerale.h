#ifndef A6C62833_A7E3_461B_B74D_D0AC0C2559AE
#define A6C62833_A7E3_461B_B74D_D0AC0C2559AE

#include "Compilateur/Visiteur/MacroGenerationVisiteur.h"
#include "Compilateur/Visiteur/Interfaces/IVisiteur.h"

CLASS_NOEUD
class NoeudInclude;

class VisiteurBaseGenerale : public IVisiteur
{
public:
    DECLARER_METHODES_VISITEUR
    void visiter(NoeudInclude* noeudInclude) override;
};

#endif /* A6C62833_A7E3_461B_B74D_D0AC0C2559AE */
