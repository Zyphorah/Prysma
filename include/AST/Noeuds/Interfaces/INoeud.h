#pragma once

#include "IExpression.h"
#include <memory>

/**
 * @interface INoeud
 * @brief Interface pour les nœuds d'une arbre d'expression
 * Hérite de IExpression
 */
class INoeud : public IExpression {
public:
    virtual ~INoeud() = default;
    
    /**
     * @brief Ajoute deux expressions comme enfants gauche et droit
     * @param gauche Expression enfant gauche
     * @param droite Expression enfant droit
     * @return Référence au nœud courant
     */
    virtual std::shared_ptr<INoeud> ajouterExpression(
        std::shared_ptr<IExpression> gauche, 
        std::shared_ptr<IExpression> droite
    ) = 0;
};
