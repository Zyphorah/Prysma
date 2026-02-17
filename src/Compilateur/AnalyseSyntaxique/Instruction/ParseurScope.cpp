#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/AnalyseSyntaxique/Instruction/ParseurScope.h"
#include "Compilateur/AST/Noeuds/NoeudScope.h"

ParseurScope::ParseurScope(IConstructeurArbre* constructeurArbreInstruction) : _constructeurArbreInstruction(constructeurArbreInstruction)
{}

ParseurScope::~ParseurScope()
{}

std::shared_ptr<INoeud> ParseurScope::parser(std::vector<Token>& tokens, int& index)
{
    std::shared_ptr<IInstruction> noeudScope = std::make_shared<NoeudScope>();
    consommer(tokens, index, TOKEN_SCOPE, "Erreur : 'scope' attendu");
    consommer(tokens, index, TOKEN_ACCOLADE_OUVERTE, "Erreur : '{' attendu après 'scope'");
    consommerEnfantCorps(tokens,index,noeudScope,_constructeurArbreInstruction, TOKEN_ACCOLADE_FERMEE);
     
    return noeudScope;
}

