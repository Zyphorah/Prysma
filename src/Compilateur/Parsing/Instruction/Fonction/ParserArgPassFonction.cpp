#include "Compilateur/Parsing/Instruction/Fonction/ParserArgPassFonction.h"
#include "Compilateur/AST/Noeuds/Fonction/NoeudArgPassFonction.h"
#include "Compilateur/Lexer/TokenType.h"
#include <memory>
#include <utility>

ParserArgPassFonction::ParserArgPassFonction(std::shared_ptr<RegistreVariable> registreVariable, std::shared_ptr<RegistreArgument> registreArgument)
{
    _registreVariable = std::move(registreVariable);
    _registreArgument = std::move(registreArgument);
}


ParserArgPassFonction::~ParserArgPassFonction()
{

}

// Exemple : pass a
std::shared_ptr<INoeud> ParserArgPassFonction::parser(std::vector<Token>& tokens, int& index,[[maybe_unused]] ConstructeurArbreInstruction* constructeurArbreInstruction)
{
    consommer(tokens,index,TOKEN_PASS,"Erreur: ce n'est pas le bon token, 'pass' ");
    Token nomVariable = consommer(tokens,index,TOKEN_IDENTIFIANT,"Erreur: identifiant invalide! ");

    return make_shared<NoeudArgPassFonction>(_registreVariable, nomVariable, _registreArgument);
}