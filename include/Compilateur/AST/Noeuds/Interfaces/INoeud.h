#pragma once

#include "Compilateur/Visiteur/Interfaces/IVisiteur.h"
#include <llvm/IR/Value.h>

class INoeud {
public:
    virtual ~INoeud() = default;

    virtual void accept(IVisiteur* visiteur) = 0;
};
