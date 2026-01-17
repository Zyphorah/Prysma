#pragma once

#include "AST/Noeuds/Interfaces/IRegistreSymbole.h"
#include <map>
#include <memory>

/**
 * @class RegistreSymbole
 * @brief Implémentation du registre des opérateurs mathématiques
 * Utilise une map ordonnée pour préserver l'ordre d'enregistrement
 */
class RegistreSymbole : public IRegistreSymbole {
private:
    // LinkedMap conserve l'ordre d'insertion
    std::map<char, std::function<std::shared_ptr<INoeud>()>> _carteSymboles;

public:
    /**
     * @brief Enregistre un opérateur
     * @param symbole Le caractère de l'opérateur
     * @param fournisseur Fonction créant un nœud d'opération
     */
    void enregistrer(
        char symbole, 
        std::function<std::shared_ptr<INoeud>()> fournisseur
    ) override;
    
    /**
     * @brief Obtient un nœud d'opération pour un symbole
     * @param symbole Le caractère de l'opérateur
     * @return Un nouveau nœud d'opération
     */
    std::shared_ptr<INoeud> recupererNoeud(char symbole) override;
    
    /**
     * @brief Vérifie si un caractère est un opérateur connu
     * @param symbole Le caractère à vérifier
     * @return true si c'est un opérateur connu
     */
    bool estOperateur(char symbole) const override;
    
    /**
     * @brief Obtient l'ensemble des symboles enregistrés
     * @return Ensemble des symboles d'opérateurs
     */
    std::set<char> obtenirSymboles() const override;
};
