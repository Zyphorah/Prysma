#pragma once

/**
 * @interface INoeud
 * @brief Interface pour toute expression mathématique pouvant être résolue
 */
#include <llvm/IR/Value.h>
class INoeud {
public:
    virtual ~INoeud() = default;
    
    /**
     * @brief C'est un noeud qui ce charge de la génération du code LLVM 
     * @return La valeur LLVM 
     */
    virtual llvm::Value* genCode() = 0;
};
