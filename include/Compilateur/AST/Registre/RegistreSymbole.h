#pragma once

#include "Compilateur/AST/Registre/Interfaces/IRegistreSymbole.h"
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
    std::map<TokenType, std::function<std::shared_ptr<IExpression>()>> _carteSymboles;

public:
    /**
     * @brief Enregistre un opérateur
     * @param symbole Le caractère de l'opérateur
     * @param fournisseur Fonction créant un nœud d'opération
     */
    void enregistrer(
        TokenType symbole, 
        std::function<std::shared_ptr<IExpression>()> fournisseur
    ) override;
    
    /**
     * @brief Obtient un nœud d'opération pour un symbole
     * @param symbole Le caractère de l'opérateur
     * @return Un nouveau nœud d'opération
     */
    std::shared_ptr<IExpression> recupererNoeud(TokenType symbole) override;
    
    /**
     * @brief Vérifie si un caractère est un opérateur connu
     * @param symbole Le caractère à vérifier
     * @return true si c'est un opérateur connu
     */
    bool estOperateur(TokenType symbole) const override;
    
    /**
     * @brief Obtient l'ensemble des symboles enregistrés
     * @return Ensemble des symboles d'opérateurs
     */
    std::set<TokenType> obtenirSymboles() const override;
};
