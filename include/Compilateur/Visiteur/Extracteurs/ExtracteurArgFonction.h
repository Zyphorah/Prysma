#ifndef F18431EF_5246_41F8_BED6_A8C2574165E4
#define F18431EF_5246_41F8_BED6_A8C2574165E4

#include "Compilateur/Visiteur/VisiteurBaseGenerale.h"

class NoeudArgFonction;

class ExtracteurArgFonction : public VisiteurBaseGenerale {
public:
    using VisiteurBaseGenerale::visiter;
    void visiter(NoeudArgFonction* noeud) override;
    [[nodiscard]] auto getArg() const -> NoeudArgFonction* { return arg; }

private:
    NoeudArgFonction* arg = nullptr;
};

#endif /* F18431EF_5246_41F8_BED6_A8C2574165E4 */
