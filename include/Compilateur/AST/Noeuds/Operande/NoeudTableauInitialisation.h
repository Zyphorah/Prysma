#ifndef CA289681_8429_4E5D_807A_243BEDD63664
#define CA289681_8429_4E5D_807A_243BEDD63664

#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include <memory>
#include <vector>

class NoeudTableauInitialisation : public INoeud {
private:
    std::vector<INoeud*> _elements;

public:
    explicit NoeudTableauInitialisation(std::vector<INoeud*> elements);
    ~NoeudTableauInitialisation() override = default;

    void accept(IVisiteur* visiteur) override;

    const std::vector<INoeud*>& getElements() const { return _elements; }
    int getTaille() const { return static_cast<int>(_elements.size()); }
};

#endif /* CA289681_8429_4E5D_807A_243BEDD63664 */
