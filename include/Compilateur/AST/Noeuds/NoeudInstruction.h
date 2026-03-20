#ifndef CE440E6B_95E4_4EC8_8C66_1916C9716A77
#define CE440E6B_95E4_4EC8_8C66_1916C9716A77
#include "Compilateur/AST/Noeuds/Interfaces/IInstruction.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include <vector>

enum class NoeudTypeGenere;

class NoeudInstruction: public IInstruction
{
private: 
    std::vector<INoeud*> enfants; 
public: 

    NoeudInstruction();

    NoeudInstruction(const NoeudInstruction& other);
    auto operator=(const NoeudInstruction& other) -> NoeudInstruction&;
    NoeudInstruction(NoeudInstruction&& other) noexcept;
    auto operator=(NoeudInstruction&& other)  noexcept -> NoeudInstruction&;
     ~NoeudInstruction() override = default;

    void accept(IVisiteur* visiteur) override;
    void ajouterInstruction(INoeud* enfant) override;
    
    [[nodiscard]] auto getTypeGenere() const -> NoeudTypeGenere override;
    
    [[nodiscard]] auto getEnfants() const -> const std::vector<INoeud*>& { return enfants; }
};

#endif /* CE440E6B_95E4_4EC8_8C66_1916C9716A77 */
