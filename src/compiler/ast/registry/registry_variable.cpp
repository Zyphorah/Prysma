//===-- registry_variable.cpp -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#include "compiler/ast/registry/stack/registry_variable.h"
#include "compiler/lexer/lexer.h"
#include "compiler/manager_error.h"
#include <llvm-18/llvm/ADT/StringMap.h>
#include <llvm-18/llvm/IR/Instructions.h>
#include <llvm-18/llvm/Support/FormatVariadic.h>
#include <stack>
#include <string>

RegistryVariable::RegistryVariable()
{
    _variables.emplace();
}

RegistryVariable::~RegistryVariable()
= default;

// Register a variable, throws if already declared in the current scope
void RegistryVariable::registerVariable(const Token& token, Symbol symbol)
{
    if(!_variables.empty())
    {
        auto it = _variables.top().find(token.value);
        if (it != _variables.top().end())
        {
            throw CompilationError(llvm::formatv("Variable '{0}' already declared", token.value).str(), Line(token.line), Column(token.column));
        }
        _variables.top()[token.value] = symbol;
    }
}

// Retrieve a variable, throws if not found
auto RegistryVariable::getVariable(const Token& token) -> Symbol
{
    if(_variables.empty())
    {
        throw CompilationError(llvm::formatv("The variable stack is empty! Variable not available: '{0}'", token.value).str(), Line(token.line), Column(token.column));
    }

    std::stack<llvm::StringMap<Symbol>> tempStack = _variables;
    while(!tempStack.empty())
    {
        auto it = tempStack.top().find(token.value);
        if (it != tempStack.top().end())
        {
            return it->second;
        }
        tempStack.pop();
    }
    throw CompilationError(llvm::formatv("Variable '{0}' not declared", token.value).str(), Line(token.line), Column(token.column));
}

// Push a new variable scope
void RegistryVariable::push()
{
    _variables.emplace();
}

// Pop the current variable scope
void RegistryVariable::pop()
{
    if (_variables.size() > 1)
    {
        _variables.pop();
    }
}

// Clear the top variable scope
void RegistryVariable::clearTop()
{
    if (!_variables.empty()) {
        _variables.top().clear();
    }
}

// Check if a variable exists in any scope
auto RegistryVariable::variableExists(const std::string& name) -> bool
{
    if(_variables.empty()) { return false;}
    
    std::stack<llvm::StringMap<Symbol>> tempStack = _variables;
    while(!tempStack.empty())
    {
        auto it = tempStack.top().find(name);
        if (it != tempStack.top().end()) { return true;}
        tempStack.pop();
    }
    return false;
}
