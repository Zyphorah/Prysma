 #include "Compilateur/AST/Registre/registreVariable.h"
#include "Compilateur/Lexer/Lexer.h"
#include <llvm-18/llvm/IR/Instructions.h>
#include <stdexcept>
#include <utility>


RegistreVariable::RegistreVariable()
{
}

RegistreVariable::~RegistreVariable()
{
}

void RegistreVariable::enregistrer(const Token& token, shared_ptr<llvm::AllocaInst> instance )
{
    auto iterateur = variables.find(token.value);
    if (iterateur != variables.end())
    {
        throw std::runtime_error("La variable que vous essayer d'enregistrer existe déjà !");
    }
    variables[token.value] = std::move(instance);
}


std::shared_ptr<llvm::AllocaInst> RegistreVariable::recupererVariables(const Token& token)
{
    auto iterateur = variables.find(token.value);
    if (iterateur != variables.end())
    {
        return iterateur->second;
    }
    throw std::runtime_error("Aucune variable de ce nom n'existe ! ");
    return nullptr;
}