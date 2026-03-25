#pragma once

#include "INode.h"

/**
 * @interface IExpression
 * @brief Interface pour les nœuds d'une tree d'expression
 * Hérite de INode
 */
class IExpression : public INode {
public:
    IExpression() = default;
    ~IExpression() override = default;

    IExpression(const IExpression&) = delete;
    auto operator=(const IExpression&) -> IExpression& = delete;
    IExpression(IExpression&&) = delete;
    auto operator=(IExpression&&) -> IExpression& = delete;
    
    /**
     * @brief Ajoute deux expressions comme childs gauche et droit
     * @param gauche Expression child gauche
     * @param droite Expression child droit
     * @return Référence au nœud courant
     */
    virtual auto ajouterExpression(
        INode* gauche, 
        INode* droite
    ) -> IExpression* = 0;
};
