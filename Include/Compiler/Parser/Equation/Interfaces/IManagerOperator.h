#pragma once

#include "Compiler/Lexer/Lexer.h"
#include <vector>


class IManagerOperateur;

/**
 * @interface IManagerOperateur
 * @brief Interface pour la chaîne de responsabilité des opérateurs
 */
class IManagerOperateur {
public:
    IManagerOperateur() = default;

    IManagerOperateur(const IManagerOperateur&) = delete;
    auto operator=(const IManagerOperateur&) -> IManagerOperateur& = delete;
    IManagerOperateur(IManagerOperateur&&) = delete;
    auto operator=(IManagerOperateur&&) -> IManagerOperateur& = delete;

    virtual ~IManagerOperateur() = default;
    
    /**
     * @brief Définit le prochain maillon de la chaîne
     * @param suivant Le prochain manager
     */
    virtual void definirSuivant(IManagerOperateur* suivant) = 0;
    
    /**
     * @brief Traite la recherche d'un opérateur
     * @param equation L'équation à traiter
     * @return L'index de l'opérateur trouvé, ou -1
     */
    virtual auto traiter(const std::vector<Token>& equation) -> int = 0;
};
