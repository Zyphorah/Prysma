#include "Compilateur/AST/Registre/Pile/RetourContexteCompilation.h"
#include <stack>
#include <stdexcept>


IType* RetourContexteCompilation::recupererContext()
{
    return _contexte.top();
}

void RetourContexteCompilation::piler(IType* token)
{
    _contexte.push(token);
}

void RetourContexteCompilation::depiler()
{
    if(_contexte.empty())
    {
        throw std::runtime_error("La pile est déjà vide! ");
    }
    _contexte.pop();
}