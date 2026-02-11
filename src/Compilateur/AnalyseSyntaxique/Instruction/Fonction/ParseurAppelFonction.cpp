#include "Compilateur/AST/Interfaces/IConstructeurArbre.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Fonction/ParseurAppelFonction.h"
#include "Compilateur/AST/Noeuds/Fonction/NoeudAppelFonction.h"
#include "Compilateur/Builder/Equation/ConstructeurEquationFlottante.h"
#include <memory>


ParseurAppelFonction::ParseurAppelFonction()
{
}

ParseurAppelFonction::~ParseurAppelFonction()
{
    
}

//call nomFonction()
std::shared_ptr<INoeud> ParseurAppelFonction::parser(std::vector<Token>& tokens, int& index, IConstructeurArbre* constructeurArbre) 
{

    consommer(tokens,index,TOKEN_CALL,"Erreur: ce n'est pas le token call, la fonction est invalid! 'call'");
    Token nomFonction = consommer(tokens,index,TOKEN_IDENTIFIANT,"Erreur: l'identifiant est invalid");
    std::shared_ptr<IInstruction> parent = make_shared<NoeudAppelFonction>(nomFonction);
    consommer(tokens,index,TOKEN_PAREN_OUVERTE, "Erreur: le token est invalide!'('");
    
    std::shared_ptr<ConstructeurEquationFlottante> constructeurArbreEquation = std::make_shared<ConstructeurEquationFlottante>(constructeurArbre);
    consommerEnfantCorps(tokens, index, parent, constructeurArbreEquation->recupererConstructeurArbre(), TOKEN_PAREN_FERMEE);
    
    consommer(tokens,index,TOKEN_PAREN_FERMEE, "Erreur: le token est invalide!')'");
    consommer(tokens,index,TOKEN_POINT_VIRGULE, "Erreur: le token est invalide!';'");

    return parent; 
}