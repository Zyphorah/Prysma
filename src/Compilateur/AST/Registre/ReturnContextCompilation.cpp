#include "Compilateur/AST/Registre/Pile/ReturnContextCompilation.h"
#include "Compilateur/Lexer/TokenType.h"
#include <stack>
#include <stdexcept>


TokenType ReturnContextCompilation::recupererContext()
{
    return _contexte.top();
}

void ReturnContextCompilation::piler(const TokenType& token)
{
    _contexte.push(token);
}

void ReturnContextCompilation::depiler()
{
    if(_contexte.empty())
    {
        throw std::runtime_error("La pile est déjà vide! ");
    }
    _contexte.pop();
}