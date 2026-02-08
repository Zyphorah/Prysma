#include "Compilateur/Parsing/Instruction/Variable/ParseurRefVariable.h"
#include "Compilateur/AST/Noeuds/Variable/NoeudRefVariable.h"
#include "Compilateur/Lexer/TokenType.h"
#include <memory>

ParseurRefVariable::ParseurRefVariable()
{
}

ParseurRefVariable::~ParseurRefVariable()
{
}

// Exemple ref variable
std::shared_ptr<INoeud> ParseurRefVariable::parser(std::vector<Token>& tokens, int& index, ConstructeurArbreInstruction* constructeurArbreInstruction) 
{
    consommer(tokens, index, TOKEN_REF, "Erreur : 'ref' attendu");
    
    Token nomToken = consommer(tokens, index, TOKEN_IDENTIFIANT, "Erreur : nom de variable attendu après 'ref'");
    std::string nomVariable = nomToken.value;
    
    return std::make_shared<NoeudRefVariable>(nomVariable);
}