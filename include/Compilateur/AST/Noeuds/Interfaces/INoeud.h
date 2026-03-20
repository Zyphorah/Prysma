#pragma once

#include <llvm/IR/Value.h>

class IVisiteur;
enum class NoeudTypeGenere;

class INoeud {
public:
    INoeud() = default;
    virtual ~INoeud() = default;

    INoeud(const INoeud&) = delete;
    auto operator=(const INoeud&) -> INoeud& = delete;
    INoeud(INoeud&&) = delete;
    auto operator=(INoeud&&) -> INoeud& = delete;

    virtual void accept(IVisiteur* visiteur) = 0;
    
    [[nodiscard]] virtual auto getTypeGenere() const -> NoeudTypeGenere = 0;
};
