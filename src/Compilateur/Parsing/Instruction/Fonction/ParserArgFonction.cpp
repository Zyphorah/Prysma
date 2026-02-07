#include "Compilateur/Parsing/Instruction/Fonction/ParserArgFonction.h"
#include "Compilateur/Lexer/TokenCategories.h"
#include "Compilateur/Lexer/TokenType.h"
#include <stdexcept>
#include "Compilateur/AST/Noeuds/Fonction/NoeudArgFonction.h"

ParserArgFonction::ParserArgFonction()
{

}

ParserArgFonction::~ParserArgFonction()
{
}

std::shared_ptr<INoeud> ParserArgFonction::parser(std::vector<Token>& tokens, int& index,[[maybe_unused]] ConstructeurArbreInstruction* constructeurArbreInstruction) 
{

    consommer(tokens,index,TOKEN_ARG,"Erreur: le token n'est pas 'arg' !");

    TokenType type; 
    if(TokenCategories::TYPES.count(tokens[index].type) != 0U)
    {
        type = tokens[index].type;
        index++;
    }
    else {
        throw std::runtime_error("Erreur: ce n'est pas un type valide! 'float', 'int', 'bool' etc.");
    }
    Token nom = consommer(tokens,index,TOKEN_IDENTIFIANT,"Erreur: ce n'est pas un identifiant!");

    return make_shared<NoeudArgFonction>(nullptr, nom.type, type);
}