#pragma once

#include <set>
#include <functional>
#include <memory>

// Forward declaration
class INoeud;

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
        char symbole, 
        std::function<std::shared_ptr<INoeud>()> supplier
    ) = 0;
    
    /**
     * @brief Obtient un nœud d'opération pour un symbole
     * @param symbole Le caractère de l'opérateur
     * @return Un nouveau nœud d'opération
     */
    virtual std::shared_ptr<INoeud> recupererNoeud(char symbole) = 0;
    
    /**
     * @brief Vérifie si un caractère est un opérateur connu
     * @param symbole Le caractère à vérifier
     * @return true si c'est un opérateur connu
     */
    virtual bool estOperateur(char symbole) const = 0;
    
    /**
     * @brief Obtient l'ensemble des symboles enregistrés
     * @return Ensemble des symboles d'opérateurs
     */
    virtual std::set<char> obtenirSymboles() const = 0;
};
