#include "Compiler/AST/Registry/Stack/RegistryVariable.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/ManagerError.h"
#include <llvm-18/llvm/IR/Instructions.h>
#include <map>
#include <stack>
#include <string>
#include <utility>

RegistryVariable::RegistryVariable()
{
    _variables.emplace();
}

RegistryVariable::~RegistryVariable()
= default;

void RegistryVariable::enregistryr(const Token& token, Symbole symbole )
{
    if(!_variables.empty())
    {
        auto iterateur = _variables.top().find(token.value);
        if (iterateur != _variables.top().end())
        {
            throw ErrorCompilation("Variable '" + token.value + "' déjà déclarée", Ligne(token.ligne), Colonne(token.colonne));
        }
         _variables.top()[token.value] = symbole;
    }
}

Symbole RegistryVariable::recupererVariables(const Token& token)
{
    if(_variables.empty())
    {
        throw ErrorCompilation("La pile des variables est vide ! Variable non disponible : '" + token.value + "'", Ligne(token.ligne), Colonne(token.colonne));
    }

    std::stack<std::map<std::string, Symbole>> tempStack = _variables;
    while(!tempStack.empty())
    {
        auto iterateur = tempStack.top().find(token.value);
        if (iterateur != tempStack.top().end())
        {
            return iterateur->second;
        }
        tempStack.pop();
    }
    throw ErrorCompilation("Variable '" + token.value + "' non déclarée", Ligne(token.ligne), Colonne(token.colonne));
}

void RegistryVariable::piler()
{
    _variables.emplace();
}

void RegistryVariable::depiler()
{
    if (_variables.size() > 1)
    {
        _variables.pop();
    }
}

void RegistryVariable::viderTop()
{
    if (!_variables.empty()) {
        _variables.top().clear();
    }
}

bool RegistryVariable::existeVariable(const std::string& nom)
{
    if(_variables.empty()) { return false;}
    
    std::stack<std::map<std::string, Symbole>> tempStack = _variables;
    while(!tempStack.empty())
    {
        auto iterateur = tempStack.top().find(nom);
        if (iterateur != tempStack.top().end()) { return true;}
        tempStack.pop();
    }
    return false;
}