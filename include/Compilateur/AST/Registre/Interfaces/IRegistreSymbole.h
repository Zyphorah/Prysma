#pragma once

#include <set>
#include <functional>
#include <memory>
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
    virtual ~IRegistreSymbole() = default;

    virtual void enregistrer(TokenType symbole, std::function<IExpression*(Token)> fournisseur) = 0;
    virtual IExpression* recupererNoeud(Token token) = 0;
    
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
