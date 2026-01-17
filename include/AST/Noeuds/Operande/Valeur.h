#pragma once

#include "AST/Noeuds/Interfaces/IExpression.h"

/**
 * @class Valeur
 * @brief Représente une valeur numérique constante
 */
class Valeur : public IExpression {
private:
    double _valeur;

public:
    /**
     * @brief Constructeur
     * @param valeur La valeur numérique
     */
    explicit Valeur(double valeur);
    
    /**
     * @brief Retourne la valeur stockée
     * @return La valeur
     */
    double resoudre() override;
};
