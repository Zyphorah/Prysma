#include "Compilateur/Parsing/Instruction/Variable/ParseurDeclaration.h"
#include "Compilateur/AST/Noeuds/Variable/NoeudDeclaration.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/Parsing/Equation/ParseurEquation.h"
#include <memory>

ParseurDeclaration::ParseurDeclaration(std::shared_ptr<LLVMBackend> backend)
    : _backend(std::move(backend))
{
}

ParseurDeclaration::~ParseurDeclaration()
{
}

std::shared_ptr<INoeud> ParseurDeclaration::parser(std::vector<Token>& tokens, int& index, std::shared_ptr<ConstructeurArbreInstruction> constructeurArbreInstruction)
{
    consommer(tokens, index, TOKEN_TYPE_FLOAT, "Erreur : 'float' attendu après 'var'");
    
    Token nomToken = consommer(tokens, index, TOKEN_IDENTIFIANT, "Erreur : nom de variable attendu");
    std::string nomVariable = nomToken.value;
    
    consommer(tokens, index, TOKEN_EGAL, "Erreur : '=' attendu après le nom de variable");
    
    ParseurEquation parseurEquation(_backend);
    std::shared_ptr<INoeud> expression = parseurEquation.parser(tokens, index, constructeurArbreInstruction);

    return std::make_shared<NoeudDeclaration>(
        _backend, 
        nomVariable, 
        _backend->getBuilder().getFloatTy(), 
        expression->genCode() 
    );
}
