#include "Compilateur/Parsing/Instruction/Variable/ParseurAffectation.h"
#include "Compilateur/AST/Noeuds/Variable/NoeudAffectation.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/Parsing/Equation/ParseurEquation.h"
#include "Compilateur/AST/Registre/RegistreVariable.h"
#include <llvm-18/llvm/IR/Instructions.h>
#include <memory>
#include <utility>

ParseurAffectation::ParseurAffectation(std::shared_ptr<LLVMBackend> backend, std::shared_ptr<RegistreVariable> registreVariable,std::shared_ptr<RegistreType> registreType)
    : _backend(std::move(backend)), _registreVariable(std::move(registreVariable)), _registreType(std::move(registreType))
{
}

ParseurAffectation::~ParseurAffectation()
{
}

std::shared_ptr<INoeud> ParseurAffectation::parser(std::vector<Token>& tokens, int& index, ConstructeurArbreInstruction* constructeurArbreInstruction)
{
    consommer(tokens, index, TOKEN_AFF, "Erreur : l'affectation est attendu 'aff'");
    Token nomToken = consommer(tokens, index, TOKEN_IDENTIFIANT, "Erreur : nom de variable attendu");
    std::string nomVariable = nomToken.value;
    
    consommer(tokens, index, TOKEN_EGAL, "Erreur : '=' attendu");
    
    ParseurEquation parseurEquation(_backend, _registreVariable);
    std::shared_ptr<INoeud> expression = parseurEquation.parser(tokens, index, constructeurArbreInstruction);

    return std::make_shared<NoeudAffectation>(_backend, nomVariable, expression, _registreVariable, nomToken);
}
