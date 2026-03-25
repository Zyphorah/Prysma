#ifndef A58A7095_E8A1_4CAC_A3A8_3136BE3D2B27
#define A58A7095_E8A1_4CAC_A3A8_3136BE3D2B27

#include "Compiler/Parser/Equation/Interfaces/IManagerOperator.h"
#include "Compiler/Parser/Equation/Interfaces/IManagerParenthesis.h"
#include "Compiler/Lexer/Lexer.h"
#include <vector>


class ManagerOperateur;

/**
 * @class ChainOfResponsibility
 * @brief Gère la chaîne de responsabilité pour la détection des opérateurs
 * Détermine la priorité des opérateurs
 */
class ChainOfResponsibility {
private:
    IManagerOperateur* _debut;
    IManagerParenthese* _managerParenthese;
    std::vector<ManagerOperateur*> _operateurs;

public:
    /**
     * @brief Builder
     * @param managerParenthese Manager de gestion des parenthèses
     * @param operateurs Vecteur des managers d'opérateurs en ordre de priorité
     */
    ChainOfResponsibility(
        IManagerParenthese* managerParenthese, 
        std::vector<ManagerOperateur*> operateurs
    );
    
    /**
     * @brief Destructor
     */
    ~ChainOfResponsibility();

    ChainOfResponsibility(const ChainOfResponsibility&) = delete;
    auto operator=(const ChainOfResponsibility&) -> ChainOfResponsibility& = delete;
    ChainOfResponsibility(ChainOfResponsibility&&) = delete;
    auto operator=(ChainOfResponsibility&&) -> ChainOfResponsibility& = delete;
    
    /**
     * @brief Trouve le prochain opérateur à traiter selon la priorité
     * @param equation L'équation à analyser
     * @return L'index de l'opérateur, ou -1 si aucun trouvé
     */
    [[nodiscard]] auto trouverOperateur(const std::vector<Token>& equation) const -> int;
};


#endif /* A58A7095_E8A1_4CAC_A3A8_3136BE3D2B27 */
