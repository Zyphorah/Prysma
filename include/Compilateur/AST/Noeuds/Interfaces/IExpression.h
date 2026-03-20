#pragma once

#include "INoeud.h"

/**
 * @interface IExpression
 * @brief Interface pour les nœuds d'une arbre d'expression
 * Hérite de INoeud
 */
class IExpression : public INoeud {
public:
    IExpression() = default;
    ~IExpression() override = default;

    IExpression(const IExpression&) = delete;
    auto operator=(const IExpression&) -> IExpression& = delete;
    IExpression(IExpression&&) = delete;
    auto operator=(IExpression&&) -> IExpression& = delete;
    
    /**
     * @brief Ajoute deux expressions comme enfants gauche et droit
     * @param gauche Expression enfant gauche
     * @param droite Expression enfant droit
     * @return Référence au nœud courant
     */
    virtual auto ajouterExpression(
        INoeud* gauche, 
        INoeud* droite
    ) -> IExpression* = 0;
};
