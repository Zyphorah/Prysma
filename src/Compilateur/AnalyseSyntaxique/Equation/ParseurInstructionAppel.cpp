#include "Compilateur/AnalyseSyntaxique/Equation/ParseurInstructionAppel.h"

#include "Compilateur/AnalyseSyntaxique/Instruction/Fonction/ParseurAppelFonction.h"
#include "Compilateur/Lexer/TokenType.h"

ParseurInstructionAppel::ParseurInstructionAppel(IConstructeurArbre* constructeurArbreEquation)
    : _constructeurArbreEquation(constructeurArbreEquation)
{
}

ParseurInstructionAppel::~ParseurInstructionAppel()
{
}

INoeud* ParseurInstructionAppel::parser(std::vector<Token>& tokens, int& index)
{
    ParseurAppelFonction parseurAppel(_constructeurArbreEquation);
    INoeud* noeud = parseurAppel.parser(tokens, index);
    
    consommer(tokens, index, TOKEN_POINT_VIRGULE, "Erreur : ';' attendu à la fin de l'appel de fonction");
    
    return noeud;
}
