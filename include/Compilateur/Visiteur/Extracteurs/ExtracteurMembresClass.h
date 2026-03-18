#ifndef D3A3E436_A6F0_4651_9DE8_DD58645AE33C
#define D3A3E436_A6F0_4651_9DE8_DD58645AE33C

#include "Compilateur/Visiteur/VisiteurBaseGenerale.h"
#include <vector>
#include <string>

class NoeudDeclarationFonction;
class NoeudDeclarationVariable;
class NoeudClass;

class ExtracteurMembresClass : public VisiteurBaseGenerale {
public:
    std::vector<NoeudDeclarationFonction*> methodes;
    std::vector<NoeudDeclarationVariable*> variables;
    std::string nomClass;
    
    using VisiteurBaseGenerale::visiter;
    void visiter(NoeudDeclarationFonction* node) override;
    void visiter(NoeudDeclarationVariable* node) override;
    void visiter(NoeudClass* node) override;
};

#endif /* D3A3E436_A6F0_4651_9DE8_DD58645AE33C */
