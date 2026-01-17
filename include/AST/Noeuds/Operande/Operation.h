#pragma once

#include "AST/Noeuds/Interfaces/INoeud.h"
#include <functional>
#include <memory>

/**
 * @class Operation
 * @brief Représente une opération binaire entre deux expressions
 * Implémente le pattern Composite
 */
class Operation : public INoeud {
private:
    std::shared_ptr<IExpression> _droite;
    std::shared_ptr<IExpression> _gauche;
    std::function<double(double, double)> _operateur;

public:
    /**
     * @brief Constructeur
     * @param operateur Fonction binaire pour effectuer l'opération
     */
    explicit Operation(std::function<double(double, double)> operateur);
    
    /**
     * @brief Ajoute deux expressions comme opérandes
     * @param gauche Opérande gauche
     * @param droite Opérande droit
     * @return Référence au nœud courant
     */
    std::shared_ptr<INoeud> ajouterExpression(
        std::shared_ptr<IExpression> gauche, 
        std::shared_ptr<IExpression> droite
    ) override;
    
    /**
     * @brief Résout l'opération
     * @return Résultat de l'application de l'opérateur
     */
    double resoudre() override;
};
