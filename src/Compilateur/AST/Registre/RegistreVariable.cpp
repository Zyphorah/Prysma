#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include "Compilateur/Lexer/Lexer.h"
#include <llvm-18/llvm/IR/Instructions.h>
#include <stdexcept>
#include <utility>


RegistreVariable::RegistreVariable()
{
    // Initialiser avec un contexte global vide
    _variables.push(std::map<std::string, llvm::Value*>());
}

RegistreVariable::~RegistreVariable()
{
}

void RegistreVariable::enregistrer(const Token& token, llvm::Value* instance )
{
    auto iterateur = _variables.top().find(token.value);
    if (iterateur != _variables.top().end())
    {
        throw std::runtime_error("La variable que vous essayer d'enregistrer existe déjà !");
    }
    _variables.top()[token.value] = instance;
}

llvm::Value* RegistreVariable::recupererVariables(const Token& token)
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
    _variables.push(std::map<std::string, llvm::Value*>());
}

void RegistreVariable::depiler()
{
    if (!_variables.empty())
    {
        _variables.pop();
    }
}