#include "Compilateur/AnalyseSyntaxique/Instruction/Fonction/ParseurArgFonction.h"
#include "Compilateur/Lexer/TokenCategories.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/GestionnaireErreur.h"
#include <stdexcept>
#include "Compilateur/AST/Noeuds/Fonction/NoeudArgFonction.h"

ParseurArgFonction::ParseurArgFonction()
{

}

ParseurArgFonction::~ParseurArgFonction()
{
}

std::shared_ptr<INoeud> ParseurArgFonction::parser(std::vector<Token>& tokens, int& index,[[maybe_unused]] ConstructeurArbreInstruction* constructeurArbreInstruction) 
{

    consommer(tokens,index,TOKEN_ARG,"Erreur: le token n'est pas 'arg' !");

    TokenType type; 
    if(TokenCategories::TYPES.count(tokens[index].type) != 0U)
    {
        type = tokens[index].type;
        index++;
    }
    else {
        throw ErreurCompilation("Erreur : ce n'est pas un type valide! 'float', 'int', 'bool' etc.", tokens[index].ligne, tokens[index].colonne);
    }
    Token nom = consommer(tokens,index,TOKEN_IDENTIFIANT,"Erreur: ce n'est pas un identifiant!");

    return make_shared<NoeudArgFonction>(nullptr, nom.value, type);
}