#pragma once

#include <llvm/IR/Value.h>

class IVisiteur;
enum class NoeudTypeGenere;

class INoeud {
public:
    virtual ~INoeud() = default;

    virtual void accept(IVisiteur* visiteur) = 0;
    
    [[nodiscard]] virtual NoeudTypeGenere getTypeGenere() const = 0;
};
