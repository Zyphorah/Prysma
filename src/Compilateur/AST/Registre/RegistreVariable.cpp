#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include "Compilateur/Lexer/Lexer.h"
#include "Compilateur/GestionnaireErreur.h"
#include <llvm-18/llvm/IR/Instructions.h>
#include <utility>

RegistreVariable::RegistreVariable()
{
    _variables.push(std::map<std::string, Symbole>());
}

RegistreVariable::~RegistreVariable()
{
}

void RegistreVariable::enregistrer(const Token& token, Symbole symbole )
{
    if(!_variables.empty())
    {
        auto iterateur = _variables.top().find(token.value);
        if (iterateur != _variables.top().end())
        {
            throw ErreurCompilation("Variable '" + token.value + "' déjà déclarée", token.ligne, token.colonne);
        }
         _variables.top()[token.value] = symbole;
    }
}

Symbole RegistreVariable::recupererVariables(const Token& token)
{
    if(_variables.empty())
    {
        throw ErreurCompilation("La pile des variables est vide ! Variable non disponible : '" + token.value + "'", token.ligne, token.colonne);
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
    throw ErreurCompilation("Variable '" + token.value + "' non déclarée", token.ligne, token.colonne);
}

void RegistreVariable::piler()
{
    _variables.push(std::map<std::string, Symbole>());
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