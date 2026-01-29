 #include "Compilateur/AST/Registre/RegistreVariable.h"
#include "Compilateur/Lexer/Lexer.h"
#include <llvm-18/llvm/IR/Instructions.h>
#include <stdexcept>
#include <utility>


RegistreVariable::RegistreVariable()
{
    // Initialiser avec un contexte global vide
    _variables.push(std::map<std::string, shared_ptr<llvm::AllocaInst>>());
}

RegistreVariable::~RegistreVariable()
{
}

void RegistreVariable::enregistrer(const Token& token, shared_ptr<llvm::AllocaInst> instance )
{
    auto iterateur = _variables.top().find(token.value);
    if (iterateur != _variables.top().end())
    {
        throw std::runtime_error("La variable que vous essayer d'enregistrer existe déjà !");
    }
    _variables.top()[token.value] = std::move(instance);
}


std::shared_ptr<llvm::AllocaInst> RegistreVariable::recupererVariables(const Token& token)
{
    auto iterateur = _variables.top().find(token.value);
    if (iterateur != _variables.top().end())
    {
        return iterateur->second;
    }
    throw std::runtime_error("Aucune variable de ce nom n'existe ! ");
    return nullptr;
}

void RegistreVariable::piler()
{
    _variables.push(std::map<std::string, shared_ptr<llvm::AllocaInst>>());
}

void RegistreVariable::depiler()
{
    if (!_variables.empty())
    {
        _variables.pop();
    }
}