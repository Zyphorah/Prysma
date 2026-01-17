#pragma once

#include <string>

/**
 * @interface IGestionnaireParenthese
 * @brief Gère la détection et suppression des parenthèses
 */
class IGestionnaireParenthese {
public:
    virtual ~IGestionnaireParenthese() = default;
    
    /**
     * @brief Enlève les parenthèses qui englobent toute l'expression
     * Ex: "(1+2)" -> "1+2", "(1+2)*(3+4)" reste inchangé
     * @param equation L'équation à traiter
     * @return L'équation sans parenthèses englobantes
     */
    virtual std::string enleverParenthesesEnglobantes(std::string equation) = 0;
    
    /**
     * @brief Trouve le dernier opérateur au niveau de parenthèses zéro
     * @param equation L'équation à analyser
     * @param operateur Le caractère opérateur à chercher
     * @return L'index de l'opérateur, ou -1 si non trouvé
     */
    virtual int trouverDernierAuNiveauZero(const std::string& equation, char operateur) = 0;
};
