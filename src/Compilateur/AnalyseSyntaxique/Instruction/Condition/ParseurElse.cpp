#include "Compilateur/AnalyseSyntaxique/Instruction/Condition/ParseurElse.h"
#include "Compilateur/AST/Noeuds/Interfaces/IInstruction.h"
#include "Compilateur/AST/Noeuds/Condition/NoeudElse.h"
#include "Compilateur/Lexer/TokenType.h"
#include <memory>

ParseurElse::ParseurElse()
{}

ParseurElse::~ParseurElse()
{}

std::shared_ptr<INoeud> ParseurElse::parser(std::vector<Token>& tokens, int& index, IConstructeurArbre* constructeurArbre) 
{
    std::shared_ptr<IInstruction> noeudElse = std::make_shared<NoeudElse>();

    consommer(tokens,index,TOKEN_SINON,"Erreur, le token n'est pas 'else'! ");
    consommer(tokens,index,TOKEN_ACCOLADE_OUVERTE, "Erreur, le token n'est pas '{'");
    consommerEnfantCorps(tokens,index,noeudElse,constructeurArbre,TOKEN_ACCOLADE_FERMEE);
    consommer(tokens,index,TOKEN_ACCOLADE_OUVERTE,"Erreur, le token n'est pas '}'");

    return noeudElse;
}