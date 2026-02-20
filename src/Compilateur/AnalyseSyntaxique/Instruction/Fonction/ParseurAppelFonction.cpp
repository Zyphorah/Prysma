#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/AST/Noeuds/Fonction/NoeudAppelFonction.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/Fonction/ParseurAppelFonction.h"
#include <memory>

ParseurAppelFonction::ParseurAppelFonction(IConstructeurArbre* constructeurArbreEquation) : _constructeurArbreEquation(constructeurArbreEquation)
{
}

ParseurAppelFonction::~ParseurAppelFonction()
{
}

INoeud* ParseurAppelFonction::parser(std::vector<Token>& tokens, int& index)
{

    consommer(tokens, index, TOKEN_CALL, "Erreur: 'call' attendu");
    Token nomFonction = consommer(tokens, index, TOKEN_IDENTIFIANT, "Erreur: identifiant de fonction attendu");
    consommer(tokens, index, TOKEN_PAREN_OUVERTE, "Erreur: '(' attendue");
    
    IInstruction* noeudAppel = new NoeudAppelFonction(nomFonction);
    
    consommerEnfantCorps(tokens, index, noeudAppel, _constructeurArbreEquation, TOKEN_PAREN_FERMEE);

    consommer(tokens, index, TOKEN_PAREN_FERMEE, "Erreur: ')' attendue");
    

    return noeudAppel;
}
