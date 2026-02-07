#ifndef A6143133_47EE_4E86_9715_0D00D76F1DE5
#define A6143133_47EE_4E86_9715_0D00D76F1DE5
#include "Compilateur/AST/Noeuds/NoeudInstruction.h"
#include "Compilateur/Lexer/Lexer.h"
#include <memory>
#include "Compilateur/Visiteur/AmisVisiteurs.h"

class NoeudAppelFonction : public NoeudInstruction
{
    LISTE_DES_AMIS_VISITEURS
private: 
    Token _nomFonction;

public:
    NoeudAppelFonction(Token nomFonction);
    ~NoeudAppelFonction();
    void accept(IVisiteur* visiteur) override;
};

#endif /* A6143133_47EE_4E86_9715_0D00D76F1DE5 */
