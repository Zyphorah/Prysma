//===-- return_context_compilation.cpp -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#include "compiler/ast/registry/stack/return_context_compilation.h"
#include "compiler/ast/registry/types/i_type.h"
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
