#pragma once

/**
 * @interface IExpression
 * @brief Interface pour toute expression mathématique pouvant être résolue
 */
#include <llvm-18/llvm/IR/Value.h>
class IExpression {
public:
    virtual ~IExpression() = default;
    
    /**
     * @brief Résout l'expression et retourne le résultat
     * @return La valeur numérique de l'expression
     */
    virtual llvm::Value* resoudre() = 0;
};
