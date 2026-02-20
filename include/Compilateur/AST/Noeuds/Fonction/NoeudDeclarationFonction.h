#ifndef A2837407_B466_49AE_8A29_4BFC0A5D0461
#define A2837407_B466_49AE_8A29_4BFC0A5D0461

#include "Compilateur/AST/Noeuds/NoeudInstruction.h"
#include "Compilateur/AST/Registre/Types/IType.h"
#include <string>
class NoeudDeclarationFonction : public NoeudInstruction
{
private:
    std::string _nom;
    IType* _typeRetour;

public:
    NoeudDeclarationFonction(std::string nom, IType* typeRetour);
    
    ~NoeudDeclarationFonction() = default;

    void accept(IVisiteur* visiteur) override;

    const std::string& getNom() const { return _nom; }
    IType* getTypeRetour() const { return _typeRetour; }
};

#endif /* A2837407_B466_49AE_8A29_4BFC0A5D0461 */
