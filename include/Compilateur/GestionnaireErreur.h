#ifndef GESTIONNAIRE_ERREUR_H
#define GESTIONNAIRE_ERREUR_H

#include <string>
#include <stdexcept>

class ErreurCompilation : public std::runtime_error
{
public:
    int ligne;
    int colonne;


    ErreurCompilation(const std::string& message, int lig, int col)
        : std::runtime_error(message), ligne(lig), colonne(col)
    {
    }

    virtual ~ErreurCompilation() = default;
};

#endif /* GESTIONNAIRE_ERREUR_H */
