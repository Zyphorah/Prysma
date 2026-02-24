#include "Compilateur/AnalyseSyntaxique/Instruction/Variable/ParseurUnRefVariable.h"
#include "Compilateur/AST/Noeuds/Variable/NoeudUnRefVariable.h"
#include "Compilateur/Lexer/TokenType.h"

ParseurUnRefVariable::ParseurUnRefVariable(llvm::BumpPtrAllocator& arena)
    : _arena(arena)
{
}

ParseurUnRefVariable::~ParseurUnRefVariable()
{
}

// Exemple unref variable
INoeud* ParseurUnRefVariable::parser(std::vector<Token>& tokens, int& index) 
{
    consommer(tokens, index, TOKEN_UNREF, "Erreur : 'unref' attendu");
    
    Token nomToken = consommer(tokens, index, TOKEN_IDENTIFIANT, "Erreur : nom de variable attendu après 'unref'");
    std::string nomVariable = nomToken.value;
    
    return new (_arena.Allocate(sizeof(NoeudUnRefVariable), alignof(NoeudUnRefVariable))) NoeudUnRefVariable(nomVariable);
}