#ifndef C5945E87_AA63_4AC2_BB0C_B2C07EECD147
#define C5945E87_AA63_4AC2_BB0C_B2C07EECD147
#include "Compilateur/AST/Noeuds/NoeudInstruction.h"
class NoeudElse: public NoeudInstruction
{
private: 

public: 
    NoeudElse();
    ~NoeudElse();
    
    void accept(IVisiteur* visiteur) override;
};

#endif /* C5945E87_AA63_4AC2_BB0C_B2C07EECD147 */
