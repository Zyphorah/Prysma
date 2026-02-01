#include "Compilateur/Parsing/Instruction/Variable/ParseurAffectation.h"
#include "Compilateur/AST/Noeuds/Variable/NoeudAffectation.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/Parsing/Equation/ParseurEquation.h"
#include "Compilateur/AST/Registre/RegistreVariable.h"
#include <llvm-18/llvm/IR/Instructions.h>
#include <memory>

ParseurAffectation::ParseurAffectation(std::shared_ptr<LLVMBackend> backend, std::shared_ptr<RegistreVariable> registreVariable, TokenType typeVariable)
    : _backend(std::move(backend)), _registreVariable(std::move(registreVariable)), _typeVariable(typeVariable)
{
}

ParseurAffectation::~ParseurAffectation()
{
}

std::shared_ptr<INoeud> ParseurAffectation::parser(std::vector<Token>& tokens, int& index, ConstructeurArbreInstruction* constructeurArbreInstruction)
{
    Token nomToken = consommer(tokens, index, TOKEN_IDENTIFIANT, "Erreur : nom de variable attendu");
    std::string nomVariable = nomToken.value;
    
    consommer(tokens, index, TOKEN_EGAL, "Erreur : '=' attendu");
    
    ParseurEquation parseurEquation(_backend, _typeVariable);
    std::shared_ptr<INoeud> expression = parseurEquation.parser(tokens, index, constructeurArbreInstruction);

    // Récupérer la variable existante du registre
    llvm::AllocaInst* variableExistante = nullptr;
    if (_registreVariable != nullptr) {
        try {
            llvm::Value* valeur = _registreVariable->recupererVariables(nomToken);
            variableExistante = llvm::dyn_cast<llvm::AllocaInst>(valeur);
        } catch (const std::exception& e) {
            // La variable n'existe pas
            throw std::runtime_error(std::string("Erreur : la variable '") + nomVariable + "' n'existe pas. Vous devez d'abord la déclarer avec 'var type nom = valeur;'");
        }
    }

    return std::make_shared<NoeudAffectation>(_backend, nomVariable, variableExistante, expression->genCode());
}
