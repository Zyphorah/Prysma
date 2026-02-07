#ifndef A2837407_B466_49AE_8A29_4BFC0A5D0461
#define A2837407_B466_49AE_8A29_4BFC0A5D0461

#include "Compilateur/AST/Noeuds/NoeudInstruction.h"
#include "Compilateur/Lexer/TokenType.h"
#include <string>
#include "Compilateur/Visiteur/AmisVisiteurs.h"

class NoeudDeclarationFonction : public NoeudInstruction
{
    LISTE_DES_AMIS_VISITEURS
private:
    std::string _nom;
    TokenType _typeRetourToken;

public:
    NoeudDeclarationFonction(std::string nom, TokenType typeRetour);
    
    ~NoeudDeclarationFonction() = default;

    void accept(IVisiteur* visiteur) override;
};

#endif /* A2837407_B466_49AE_8A29_4BFC0A5D0461 */
