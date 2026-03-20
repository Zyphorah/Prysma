#pragma once

#include "Compilateur/Lexer/Lexer.h"
#include <vector>


class IGestionnaireOperateur;

/**
 * @interface IGestionnaireOperateur
 * @brief Interface pour la chaîne de responsabilité des opérateurs
 */
class IGestionnaireOperateur {
public:
    IGestionnaireOperateur() = default;

    IGestionnaireOperateur(const IGestionnaireOperateur&) = delete;
    auto operator=(const IGestionnaireOperateur&) -> IGestionnaireOperateur& = delete;
    IGestionnaireOperateur(IGestionnaireOperateur&&) = delete;
    auto operator=(IGestionnaireOperateur&&) -> IGestionnaireOperateur& = delete;

    virtual ~IGestionnaireOperateur() = default;
    
    /**
     * @brief Définit le prochain maillon de la chaîne
     * @param suivant Le prochain gestionnaire
     */
    virtual void definirSuivant(IGestionnaireOperateur* suivant) = 0;
    
    /**
     * @brief Traite la recherche d'un opérateur
     * @param equation L'équation à traiter
     * @return L'index de l'opérateur trouvé, ou -1
     */
    virtual auto traiter(const std::vector<Token>& equation) -> int = 0;
};
