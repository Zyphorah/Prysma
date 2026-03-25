#pragma once

#include <set>
#include <functional>
#include "Compiler/Lexer/TokenType.h"


#include "Compiler/Lexer/Lexer.h"

class IExpression;

/**
 * @interface IRegistrySymbole
 * @brief Registry des opérateurs mathématiques avec gestion de priorité
 * L'ordre d'enregistryment détermine la priorité:
 * - Enregistrés EN PREMIER: priorité PLUS BASSE
 * - Enregistrés EN DERNIER: priorité PLUS HAUTE
 */
class IRegistrySymbole {
public:
    IRegistrySymbole() = default;

    virtual ~IRegistrySymbole() = default;
    IRegistrySymbole(const IRegistrySymbole&) = delete;
    auto operator=(const IRegistrySymbole&) -> IRegistrySymbole& = delete;
    IRegistrySymbole(IRegistrySymbole&&) = delete;
    auto operator=(IRegistrySymbole&&) -> IRegistrySymbole& = delete;

    virtual void enregistryr(TokenType symbole, std::function<IExpression*(Token)> fournisseur) = 0;
    virtual auto recupererNode(Token token) -> IExpression* = 0;
    
    /**
     * @brief Vérifie si un caractère est un opérateur connu
     * @param symbole Le caractère à vérifier
     * @return true si c'est un opérateur connu
     */
    [[nodiscard]] virtual auto estOperateur(TokenType symbole) const -> bool = 0;
    
    /**
     * @brief Obtient l'ensemble des symboles enregistrés
     * @return Ensemble des symboles d'opérateurs
     */
    [[nodiscard]] virtual std::set<TokenType> obtenirSymboles() const = 0;
};
