#ifndef CE440E6B_95E4_4EC8_8C66_1916C9716A77
#define CE440E6B_95E4_4EC8_8C66_1916C9716A77
#include "Compilateur/AST/Noeuds/Interfaces/IInstruction.h"
#include <vector>
#include <memory>
#include "Compilateur/Visiteur/AmisVisiteurs.h"


class NoeudInstruction: public IInstruction
{
    LISTE_DES_AMIS_VISITEURS
protected: 
    std::vector<std::shared_ptr<INoeud>> enfants; 
public: 

    NoeudInstruction();
    virtual ~NoeudInstruction() = default;

    void accept(IVisiteur* visiteur) override;
    void ajouterInstruction(std::shared_ptr<INoeud> enfant) override;
    
    [[nodiscard]] const std::vector<std::shared_ptr<INoeud>>& getEnfants() const { return enfants; }
};

#endif /* CE440E6B_95E4_4EC8_8C66_1916C9716A77 */
