#ifndef C1FA2E98_E6EC_48D0_9F9D_84FB77C0023E
#define C1FA2E98_E6EC_48D0_9F9D_84FB77C0023E

#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include <string>

class NoeudUnRefVariable : public INoeud
{   
private:
    std::string _nomVariable;

public: 
    explicit NoeudUnRefVariable(const std::string& nomVariable);
    ~NoeudUnRefVariable();

    void accept(IVisiteur* visiteur) override;

    const std::string& getNomVariable() const { return _nomVariable; }
};

#endif /* C1FA2E98_E6EC_48D0_9F9D_84FB77C0023E */
