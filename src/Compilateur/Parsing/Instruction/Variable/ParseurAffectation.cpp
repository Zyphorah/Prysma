#include "Compilateur/Parsing/Instruction/Variable/ParseurAffectation.h"
#include "Compilateur/AST/Noeuds/Variable/NoeudAffectation.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/Builder/Equation/FloatEquationBuilder.h"
#include "Compilateur/Parsing/Equation/ParseurEquation.h"
#include <memory>

ParseurAffectation::ParseurAffectation(std::shared_ptr<LLVMBackend> backend)
    : _backend(std::move(backend))
{
}

ParseurAffectation::~ParseurAffectation()
{
}

std::shared_ptr<INoeud> ParseurAffectation::parser(std::vector<Token>& tokens, int& index, std::shared_ptr<ConstructeurArbreInstruction> constructeurArbreInstruction)
{
    Token nomToken = consommer(tokens, index, TOKEN_IDENTIFIANT, "Erreur : nom de variable attendu");
    std::string nomVariable = nomToken.value;
    
    consommer(tokens, index, TOKEN_EGAL, "Erreur : '=' attendu");
    
    ParseurEquation parseurEquation(_backend);
    std::shared_ptr<INoeud> expression = parseurEquation.parser(tokens, index, constructeurArbreInstruction);

    return std::make_shared<NoeudAffectation>(_backend, nomVariable, nullptr, expression->genCode());
}
