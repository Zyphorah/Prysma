#ifndef CE440E6B_95E4_4EC8_8C66_1916C9716A77
#define CE440E6B_95E4_4EC8_8C66_1916C9716A77
#include "Compilateur/AST/Noeuds/Interfaces/IInstruction.h"
#include <vector>
#include <memory>
class NoeudInstruction: public IInstruction
{
protected: 
    std::vector<INoeud*> enfants; 
public: 

    NoeudInstruction();
    virtual ~NoeudInstruction() = default;

    void accept(IVisiteur* visiteur) override;
    void ajouterInstruction(INoeud* enfant) override;
    
    [[nodiscard]] const std::vector<INoeud*>& getEnfants() const { return enfants; }
};

#endif /* CE440E6B_95E4_4EC8_8C66_1916C9716A77 */
