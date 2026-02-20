#include "Compilateur/AnalyseSyntaxique/Instruction/Fonction/ParseurArgFonction.h"
#include "Compilateur/AST/Registre/Types/IType.h"
#include "Compilateur/AnalyseSyntaxique/ParseurType.h"
#include "Compilateur/Lexer/TokenType.h"
#include <memory>
#include <stdexcept>
#include <utility>
#include "Compilateur/AST/Noeuds/Fonction/NoeudArgFonction.h"

ParseurArgFonction::ParseurArgFonction(ParseurType* constructeurType) : _constructeurType(constructeurType)
{

}

ParseurArgFonction::~ParseurArgFonction()
{
}

INoeud* ParseurArgFonction::parser(std::vector<Token>& tokens, int& index) 
{

    consommer(tokens,index,TOKEN_ARG,"Erreur: le token n'est pas 'arg' !");

    // Refactorisation utilisation du système IType 
    IType* type = _constructeurType->parser(tokens, index);

    Token nom = consommer(tokens,index,TOKEN_IDENTIFIANT,"Erreur: ce n'est pas un identifiant!");

    return new NoeudArgFonction(type, nom.value);
}