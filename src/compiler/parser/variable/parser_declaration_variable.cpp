//===-- parser_declaration_variable.cpp -------------------------------------------*- C++ -*-===//
//
// Part of the Prysma Project, under the GNU GPL v3.0 or later.
// See LICENSE at the project root for license information.
// SPDX-License-Identifier: GPL-3.0-or-later WITH Prysma-exception-1.0
//
//===----------------------------------------------------------------------===//

#ifndef PARSER_DECLARATIONVARIABLE_CPP
#define PARSER_DECLARATIONVARIABLE_CPP

#include "compiler/variable/parser_declaration_variable.h"
#include "compiler/ast/ast_genere.h"
#include "compiler/ast/nodes/interfaces/i_node.h"
#include "compiler/ast/registry/context_parser.h"
#include "compiler/ast/registry/types/i_type.h"
#include "compiler/lexer/lexer.h"
#include "compiler/lexer/token_type.h"
#include <string>
#include <vector>


ParserDeclarationVariable::ParserDeclarationVariable(ContextParser& contextParser) 
    : _contextParser(contextParser)
{}

ParserDeclarationVariable::~ParserDeclarationVariable()
= default;

auto ParserDeclarationVariable::parse(std::vector<Token>& tokens, int& index) -> INode*
{
    consume(tokens, index, TOKEN_DECL, "Error: expected type 'dec'");
    
    // Use ParserType to analyze the type (simple or array)
    IType* type = _contextParser.getTypeParser()->parse(tokens, index);
    
    Token nameToken = consume(tokens, index, TOKEN_IDENTIFIER, "Error: variable name expected");
    
    consume(tokens, index, TOKEN_EQUAL, "Error: '=' expected after variable name");
    
    INode* expression = _contextParser.getBuilderTreeEquation()->build(tokens, index);
    
    consume(tokens, index, TOKEN_SEMICOLON, "Error: ';' expected at the end of the declaration");

    return _contextParser.getBuilderTreeEquation()->allocate<NodeDeclarationVariable>(
        Token{},
        nameToken,
        type,
        expression
    );
}
#endif /* PARSER_DECLARATIONVARIABLE_CPP */




