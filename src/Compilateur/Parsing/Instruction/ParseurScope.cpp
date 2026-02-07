#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/Parsing/Instruction/ParserScope.h"
#include "Compilateur/AST/Noeuds/NoeudScope.h"

std::shared_ptr<INoeud> ParserScope::parser(std::vector<Token>& tokens, int& index, ConstructeurArbreInstruction* constructeurArbreInstruction)
{
    std::shared_ptr<IInstruction> noeudScope = std::make_shared<NoeudScope>();
    consommer(tokens, index, TOKEN_SCOPE, "Erreur : 'scope' attendu");
    consommer(tokens, index, TOKEN_ACCOLADE_OUVERTE, "Erreur : '{' attendu après 'scope'");
    consommerEnfantCorps(tokens,index,noeudScope,constructeurArbreInstruction, TOKEN_ACCOLADE_FERMEE);
     
    return noeudScope;
}

