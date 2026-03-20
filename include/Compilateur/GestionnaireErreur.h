#ifndef GESTIONNAIRE_ERREUR_H
#define GESTIONNAIRE_ERREUR_H

#include <string>
#include <stdexcept>

struct Ligne {
private:
    int value;
public:
    explicit Ligne(int val) : value(val) {}
    [[nodiscard]] auto getValue() const -> int { return value; }
};

struct Colonne {
private:
    int value;
public:
    explicit Colonne(int val) : value(val) {}
    [[nodiscard]] auto getValue() const -> int { return value; }
};

class ErreurCompilation : public std::runtime_error
{
private:
    Ligne ligne;
    Colonne colonne;

public:
    [[nodiscard]] auto getLigne() const -> int { return ligne.getValue(); }
    [[nodiscard]] auto getColonne() const -> int { return colonne.getValue(); }

    ErreurCompilation(const std::string& message, Ligne lig, Colonne col)
        : std::runtime_error(message), ligne(lig), colonne(col)
    {
    }

    ErreurCompilation(const ErreurCompilation&) = default;
    auto operator=(const ErreurCompilation&) -> ErreurCompilation& = default;
    ErreurCompilation(ErreurCompilation&&) = default;
    auto operator=(ErreurCompilation&&) -> ErreurCompilation& = default;

    ~ErreurCompilation() override = default;
};

#endif /* GESTIONNAIRE_ERREUR_H */
