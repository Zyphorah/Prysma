#include "Compiler/AST/Registry/Stack/ReturnContextCompilation.h"
#include "Compiler/AST/Registry/Types/IType.h"
#include <stack>
#include <stdexcept>

// Retrieve the current return context
auto ReturnContextCompilation::getContext() -> IType*
{
    return _context.top();
}

// Push a new return context
void ReturnContextCompilation::push(IType* token)
{
    _context.push(token);
}

// Pop the current return context
void ReturnContextCompilation::pop()
{
    if(_context.empty())
    {
        throw std::runtime_error("The stack is already empty!");
    }
    _context.pop();
}