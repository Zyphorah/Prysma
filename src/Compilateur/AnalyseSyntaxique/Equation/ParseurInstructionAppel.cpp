#include "Compilateur/AnalyseSyntaxique/Equation/ParseurInstructionAppel.h"

#include "Compilateur/AnalyseSyntaxique/Instruction/Fonction/ParseurAppelFonction.h"
#include "Compilateur/Lexer/TokenType.h"
#include <memory>

ParseurInstructionAppel::ParseurInstructionAppel()
{
}

ParseurInstructionAppel::~ParseurInstructionAppel()
{
}

std::shared_ptr<INoeud> ParseurInstructionAppel::parser(std::vector<Token>& tokens, int& index, IConstructeurArbre* constructeurArbre)
{
    ParseurAppelFonction parseurAppel(constructeurArbre);
    std::shared_ptr<INoeud> noeud = parseurAppel.parser(tokens, index, constructeurArbre);
    
    consommer(tokens, index, TOKEN_POINT_VIRGULE, "Erreur : ';' attendu à la fin de l'appel de fonction");
    
    return noeud;
}
