#ifndef PARSER_REFVARIABLE_CPP
#define PARSER_REFVARIABLE_CPP

#include "Compiler/Variable/ParserRefVariable.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/ContextParser.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include <string>
#include <vector>


ParserRefVariable::ParserRefVariable(ContextParser& contextParser) 
    : _contextParser(contextParser)
{}

ParserRefVariable::~ParserRefVariable()
=default;

// Example: ref variable
auto ParserRefVariable::parse(std::vector<Token>& tokens, int& index) -> INode* 
{
    consume(tokens, index, TOKEN_REF, "Error: 'ref' expected");
    
    Token nameToken = consume(tokens, index, TOKEN_IDENTIFIER, "Error: variable name expected after 'ref'");
    std::string variableName = nameToken.value;
    
    return _contextParser.getBuilderTreeEquation()->allocate<NodeRefVariable>(variableName);
}

#endif /* PARSER_REFVARIABLE_CPP */




