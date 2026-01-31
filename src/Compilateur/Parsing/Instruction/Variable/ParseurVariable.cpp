#include "Compilateur/Parsing/Instruction/Variable/ParseurVariable.h"
#include "Compilateur/Parsing/Instruction/Variable/ParseurDeclaration.h"
#include "Compilateur/Parsing/Instruction/Variable/ParseurAffectation.h"
#include "Compilateur/Lexer/TokenType.h"
#include <llvm-18/llvm/IR/Instructions.h>
#include <memory>
#include <utility>


ParseurVariable::ParseurVariable(std::shared_ptr<LLVMBackend> backend)
{
   _backend = std::move(backend);
}

ParseurVariable::~ParseurVariable()
{
}

// Exemple: var float teste = 34+435+4;
std::shared_ptr<INoeud> ParseurVariable::parser(std::vector<Token>& tokens, int& index, std::shared_ptr<ConstructeurArbreInstruction> constructeurArbreInstruction)
{
    consommer(tokens, index, TOKEN_VAR, "Erreur : 'var' attendu");
    
    if (index < static_cast<int>(tokens.size()) && tokens[index].type == TOKEN_TYPE_FLOAT) {
        ParseurDeclaration parseurDeclaration(_backend);
        return parseurDeclaration.parser(tokens, index, constructeurArbreInstruction);
    }

    ParseurAffectation parseurAffectation(_backend);
    return parseurAffectation.parser(tokens, index, constructeurArbreInstruction);
}