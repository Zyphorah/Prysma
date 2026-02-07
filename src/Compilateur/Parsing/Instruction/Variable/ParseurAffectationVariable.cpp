#include "Compilateur/Parsing/Instruction/Variable/ParseurAffectationVariable.h"
#include "Compilateur/AST/Noeuds/Variable/NoeudAffectationVariable.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/Parsing/Equation/ParseurEquation.h"
#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include <llvm-18/llvm/IR/Instructions.h>
#include <memory>
#include <utility>

ParseurAffectationVariable::ParseurAffectationVariable(std::shared_ptr<LLVMBackend> backend, std::shared_ptr<RegistreVariable> registreVariable,std::shared_ptr<RegistreType> registreType)
    : _backend(std::move(backend)), _registreVariable(std::move(registreVariable)), _registreType(std::move(registreType))
{
}

ParseurAffectationVariable::~ParseurAffectationVariable()
{
}

std::shared_ptr<INoeud> ParseurAffectationVariable::parser(std::vector<Token>& tokens, int& index, ConstructeurArbreInstruction* constructeurArbreInstruction)
{
    consommer(tokens, index, TOKEN_AFF, "Erreur : l'affectation est attendu 'aff'");
    Token nomToken = consommer(tokens, index, TOKEN_IDENTIFIANT, "Erreur : nom de variable attendu");
    std::string nomVariable = nomToken.value;
    
    consommer(tokens, index, TOKEN_EGAL, "Erreur : '=' attendu");
    
    ParseurEquation parseurEquation{};
    std::shared_ptr<INoeud> expression = parseurEquation.parser(tokens, index, constructeurArbreInstruction);

    return std::make_shared<NoeudAffectationVariable>(nomVariable, expression, nomToken);
}
