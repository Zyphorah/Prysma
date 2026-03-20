#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/GestionnaireErreur.h"
#include <llvm-18/llvm/IR/Instructions.h>
#include <map>
#include <stack>
#include <string>
#include <utility>

RegistreVariable::RegistreVariable()
{
    _variables.emplace();
}

RegistreVariable::~RegistreVariable()
= default;

void RegistreVariable::enregistrer(const Token& token, Symbole symbole )
{
    if(!_variables.empty())
    {
        auto iterateur = _variables.top().find(token.value);
        if (iterateur != _variables.top().end())
        {
            throw ErreurCompilation("Variable '" + token.value + "' déjà déclarée", Ligne(token.ligne), Colonne(token.colonne));
        }
         _variables.top()[token.value] = symbole;
    }
}

Symbole RegistreVariable::recupererVariables(const Token& token)
{
    if(_variables.empty())
    {
        throw ErreurCompilation("La pile des variables est vide ! Variable non disponible : '" + token.value + "'", Ligne(token.ligne), Colonne(token.colonne));
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
    throw ErreurCompilation("Variable '" + token.value + "' non déclarée", Ligne(token.ligne), Colonne(token.colonne));
}

void RegistreVariable::piler()
{
    _variables.emplace();
}

void RegistreVariable::depiler()
{
    if (_variables.size() > 1)
    {
        _variables.pop();
    }
}

void RegistreVariable::viderTop()
{
    if (!_variables.empty()) {
        _variables.top().clear();
    }
}

bool RegistreVariable::existeVariable(const std::string& nom)
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