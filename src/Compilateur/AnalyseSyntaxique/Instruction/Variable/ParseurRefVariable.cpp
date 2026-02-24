#include "Compilateur/AnalyseSyntaxique/Instruction/Variable/ParseurRefVariable.h"
#include "Compilateur/AST/Noeuds/Variable/NoeudRefVariable.h"
#include "Compilateur/Lexer/TokenType.h"

ParseurRefVariable::ParseurRefVariable(llvm::BumpPtrAllocator& arena)
    : _arena(arena)
{
}

ParseurRefVariable::~ParseurRefVariable()
{
}

// Exemple ref variable
INoeud* ParseurRefVariable::parser(std::vector<Token>& tokens, int& index) 
{
    consommer(tokens, index, TOKEN_REF, "Erreur : 'ref' attendu");
    
    Token nomToken = consommer(tokens, index, TOKEN_IDENTIFIANT, "Erreur : nom de variable attendu après 'ref'");
    std::string nomVariable = nomToken.value;
    
    return new (_arena.Allocate(sizeof(NoeudRefVariable), alignof(NoeudRefVariable))) NoeudRefVariable(nomVariable);
}