#pragma once

#include <set>
#include <functional>
#include <memory>
#include "Compilateur/Lexer/TokenType.h"

// Forward declaration
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
    virtual ~IRegistreSymbole() = default;
    
    /**
     * @brief Enregistre un opérateur
     * @param symbole Le caractère de l'opérateur
     * @param supplier Fonction créant un nœud d'opération
     */
    virtual void enregistrer(
        TokenType symbole, 
        std::function<std::shared_ptr<IExpression>()> supplier
    ) = 0;
    
    /**
     * @brief Obtient un nœud d'opération pour un symbole
     * @param symbole Le caractère de l'opérateur
     * @return Un nouveau nœud d'opération
     */
    virtual std::shared_ptr<IExpression> recupererNoeud(TokenType symbole) = 0;
    
    /**
     * @brief Vérifie si un caractère est un opérateur connu
     * @param symbole Le caractère à vérifier
     * @return true si c'est un opérateur connu
     */
    virtual bool estOperateur(TokenType symbole) const = 0;
    
    /**
     * @brief Obtient l'ensemble des symboles enregistrés
     * @return Ensemble des symboles d'opérateurs
     */
    virtual std::set<TokenType> obtenirSymboles() const = 0;
};
