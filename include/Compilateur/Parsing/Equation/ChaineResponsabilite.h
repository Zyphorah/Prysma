#ifndef A58A7095_E8A1_4CAC_A3A8_3136BE3D2B27
#define A58A7095_E8A1_4CAC_A3A8_3136BE3D2B27

#include "Compilateur/Parsing/Equation/Interfaces/IGestionnaireOperateur.h"
#include "Compilateur/Parsing/Equation/Interfaces/IGestionnaireParenthese.h"
#include <vector>


class GestionnaireOperateur;

/**
 * @class ChaineResponsabilite
 * @brief Gère la chaîne de responsabilité pour la détection des opérateurs
 * Détermine la priorité des opérateurs
 */
class ChaineResponsabilite {
private:
    IGestionnaireOperateur* _debut;
    IGestionnaireParenthese* _gestionnaireParenthese;
    std::vector<GestionnaireOperateur*> _operateurs;

public:
    /**
     * @brief Constructeur
     * @param gestionnaireParenthese Gestionnaire de gestion des parenthèses
     * @param operateurs Vecteur des gestionnaires d'opérateurs en ordre de priorité
     */
    ChaineResponsabilite(
        IGestionnaireParenthese* gestionnaireParenthese, 
        std::vector<GestionnaireOperateur*> operateurs
    );
    
    /**
     * @brief Destructor
     */
    ~ChaineResponsabilite();
    
    /**
     * @brief Trouve le prochain opérateur à traiter selon la priorité
     * @param equation L'équation à analyser
     * @return L'index de l'opérateur, ou -1 si aucun trouvé
     */
    int trouverOperateur(const std::vector<Token>& equation) const;
};


#endif /* A58A7095_E8A1_4CAC_A3A8_3136BE3D2B27 */
