#ifndef PARSER_DECLARATIONVARIABLE_CPP
#define PARSER_DECLARATIONVARIABLE_CPP

#include "Compiler/Variable/ParserDeclarationVariable.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/ContextParser.h"
#include "Compiler/AST/Registry/Types/IType.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
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
    std::string variableName = nameToken.value;
    
    consume(tokens, index, TOKEN_EQUAL, "Error: '=' expected after variable name");
    
    INode* expression = _contextParser.getBuilderTreeEquation()->build(tokens, index);
    
    consume(tokens, index, TOKEN_SEMICOLON, "Error: ';' expected at the end of the declaration");

    return _contextParser.getBuilderTreeEquation()->allocate<NodeDeclarationVariable>(
        Token{},
        variableName,
        type,
        expression
    );
}
#endif /* PARSER_DECLARATIONVARIABLE_CPP */




