#pragma once

#include <set>
#include <functional>
#include "Compilateur/Lexer/TokenType.h"


#include "Compilateur/Lexer/Lexer.h"

class IExpression;

/**
 * @interface IRegistreSymbole
 * @brief Registre des opérateurs mathématiques avec gestion de priorité
 * L'ordre d'enregistrement détermine la priorité:
 * - Enregistrés EN PREMIER: priorité PLUS BASSE
 * - Enregistrés EN DERNIER: priorité PLUS HAUTE
 */
class IRegistreSymbole {
public:
    IRegistreSymbole() = default;

    virtual ~IRegistreSymbole() = default;
    IRegistreSymbole(const IRegistreSymbole&) = delete;
    auto operator=(const IRegistreSymbole&) -> IRegistreSymbole& = delete;
    IRegistreSymbole(IRegistreSymbole&&) = delete;
    auto operator=(IRegistreSymbole&&) -> IRegistreSymbole& = delete;

    virtual void enregistrer(TokenType symbole, std::function<IExpression*(Token)> fournisseur) = 0;
    virtual auto recupererNoeud(Token token) -> IExpression* = 0;
    
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
