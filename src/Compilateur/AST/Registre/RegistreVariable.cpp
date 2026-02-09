#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/GestionnaireErreur.h"
#include <llvm-18/llvm/IR/Instructions.h>
#include <stdexcept>
#include <utility>


RegistreVariable::RegistreVariable()
{
    _variables.push(std::map<std::string, llvm::Value*>());
}

RegistreVariable::~RegistreVariable()
{
}

void RegistreVariable::enregistrer(const Token& token, llvm::Value* instance )
{
    if(!_variables.empty())
    {
        auto iterateur = _variables.top().find(token.value);
        if (iterateur != _variables.top().end())
        {
            throw ErreurCompilation("Variable '" + token.value + "' déjà déclarée", token.ligne, token.colonne);
        }
         _variables.top()[token.value] = instance;
    }
}

llvm::Value* RegistreVariable::recupererVariables(const Token& token)
{

    if(_variables.empty())
    {
        throw ErreurCompilation("La pile des variables est vide ! Variable non disponible : '" + token.value + "'", token.ligne, token.colonne);
    }

    if(!_variables.empty())
    {
        auto iterateur = _variables.top().find(token.value);
        if (iterateur != _variables.top().end())
        {
            return iterateur->second;
        }
    }
    throw ErreurCompilation("Variable '" + token.value + "' non déclarée", token.ligne, token.colonne);
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