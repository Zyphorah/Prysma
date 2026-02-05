#pragma once

#include "Compilateur/Lexer/Lexer.h"


class IGestionnaireOperateur;

/**
 * @interface IGestionnaireOperateur
 * @brief Interface pour la chaîne de responsabilité des opérateurs
 */
class IGestionnaireOperateur {
public:
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
    virtual int traiter(const std::vector<Token>& equation) = 0;
};
