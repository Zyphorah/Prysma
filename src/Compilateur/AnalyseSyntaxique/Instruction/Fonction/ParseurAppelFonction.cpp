#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/AST/Noeuds/Fonction/NoeudAppelFonction.h"
#include "Compilateur/Builder/Equation/ConstructeurEquationFlottante.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Fonction/ParseurAppelFonction.h"
#include <memory>

ParseurAppelFonction::ParseurAppelFonction()
{
}

ParseurAppelFonction::~ParseurAppelFonction()
{
}

std::shared_ptr<INoeud> ParseurAppelFonction::parser(std::vector<Token>& tokens, int& index, IConstructeurArbre* constructeurArbre)
{

    consommer(tokens, index, TOKEN_CALL, "Erreur: 'call' attendu");
    Token nomFonction = consommer(tokens, index, TOKEN_IDENTIFIANT, "Erreur: identifiant de fonction attendu");
    consommer(tokens, index, TOKEN_PAREN_OUVERTE, "Erreur: '(' attendue");
    
    std::shared_ptr<IInstruction> noeudAppel = std::make_shared<NoeudAppelFonction>(nomFonction);
    

    std::shared_ptr<ConstructeurEquationFlottante> constructeurEquation = std::make_shared<ConstructeurEquationFlottante>(constructeurArbre);
    
    consommerEnfantCorps(tokens, index, noeudAppel, constructeurEquation->recupererConstructeurArbre(), TOKEN_PAREN_FERMEE);

    consommer(tokens, index, TOKEN_PAREN_FERMEE, "Erreur: ')' attendue");
    

    return noeudAppel;
}
