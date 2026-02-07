#include "Compilateur/Parsing/Instruction/Variable/ParseurDeclarationVariable.h"
#include "Compilateur/AST/Noeuds/Variable/NoeudDeclarationVariable.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/Parsing/Equation/ParseurEquation.h"
#include "Compilateur/Lexer/TokenCategories.h"
#include <memory>
#include <iostream> 

ParseurDeclarationVariable::ParseurDeclarationVariable()
{}

ParseurDeclarationVariable::~ParseurDeclarationVariable()
{
}

std::shared_ptr<INoeud> ParseurDeclarationVariable::parser(std::vector<Token>& tokens, int& index, ConstructeurArbreInstruction* constructeurArbreInstruction)
{
    consommer(tokens,index,TOKEN_DEC,"Erreur : type attendu 'dec");
    
    // Vérifier que le token courant est un type valide
    if (TokenCategories::TYPES.find(tokens[index].type) == TokenCategories::TYPES.end()) {
        throw std::runtime_error("Erreur : type attendu après 'dec'");
    }
    Token typeToken = tokens[index];
    index++;
    
    Token nomToken = consommer(tokens, index, TOKEN_IDENTIFIANT, "Erreur : nom de variable attendu");
    std::string nomVariable = nomToken.value;
    
    consommer(tokens, index, TOKEN_EGAL, "Erreur : '=' attendu après le nom de variable");
    
    ParseurEquation parseurEquation{};
    std::shared_ptr<INoeud> expression = parseurEquation.parser(tokens, index, constructeurArbreInstruction);

    return std::make_shared<NoeudDeclarationVariable>(
        nomVariable,
        expression,
        typeToken.type
    );
}
