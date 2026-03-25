#pragma once

#include "Compiler/Lexer/Lexer.h"
#include <vector>

/**
 * @interface IManagerParenthese
 * @brief Gère la détection et suppression des parenthèses
 */
class IManagerParenthese {
public:
    IManagerParenthese() = default;
    virtual ~IManagerParenthese() = default;
    IManagerParenthese(const IManagerParenthese&) = delete;
    auto operator=(const IManagerParenthese&) -> IManagerParenthese& = delete;
    IManagerParenthese(IManagerParenthese&&) = delete;
    auto operator=(IManagerParenthese&&) -> IManagerParenthese& = delete;
    
    /**
     * @brief Enlève les parenthèses qui englobent toute l'expression
     * Ex: "(1+2)" -> "1+2", "(1+2)*(3+4)" reste inchangé
     * @param equation L'équation à traiter
     * @return L'équation sans parenthèses englobantes
     */
    virtual auto enleverParenthesesEnglobantes(const std::vector<Token>& equation) -> std::vector<Token> = 0;
    
    /**
     * @brief Trouve le dernier opérateur au niveau de parenthèses zéro
     * @param equation L'équation à analyser
     * @param operateur Le caractère opérateur à chercher
     * @return L'index de l'opérateur, ou -1 si non trouvé
     */
    virtual auto trouverDernierAuNiveauZero(const vector<Token>& equation, Token operateur) -> int = 0;
};
