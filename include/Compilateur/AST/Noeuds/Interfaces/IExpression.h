#pragma once

#include "INoeud.h"
#include <memory>

/**
 * @interface IExpression
 * @brief Interface pour les nœuds d'une arbre d'expression
 * Hérite de INoeud
 */
class IExpression : public INoeud {
public:
    virtual ~IExpression() = default;
    
    /**
     * @brief Ajoute deux expressions comme enfants gauche et droit
     * @param gauche Expression enfant gauche
     * @param droite Expression enfant droit
     * @return Référence au nœud courant
     */
    virtual IExpression* ajouterExpression(
        INoeud* gauche, 
        INoeud* droite
    ) = 0;
};
