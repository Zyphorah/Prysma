#pragma once

#include <llvm/IR/Value.h>

class IVisiteur;

class INoeud {
public:
    virtual ~INoeud() = default;

    virtual void accept(IVisiteur* visiteur) = 0;
};
