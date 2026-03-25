#include "Compiler/AST/Registry/Stack/ReturnContextCompilation.h"
#include "Compiler/AST/Registry/Types/IType.h"
#include <stack>
#include <stdexcept>


auto ReturnContextCompilation::recupererContext() -> IType*
{
    return _contexte.top();
}

void ReturnContextCompilation::piler(IType* token)
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