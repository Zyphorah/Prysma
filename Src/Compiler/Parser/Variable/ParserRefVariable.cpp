#ifndef PARSEUR_REFVARIABLE_CPP
#define PARSEUR_REFVARIABLE_CPP

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
= default;

// Exemple ref variable
auto ParserRefVariable::parser(std::vector<Token>& tokens, int& index) -> INode* 
{
    consommer(tokens, index, TOKEN_REF, "Error : 'ref' attendu");
    
    Token nomToken = consommer(tokens, index, TOKEN_IDENTIFIANT, "Error : nom de variable attendu après 'ref'");
    std::string nomVariable = nomToken.value;
    
    return _contextParser.getBuilderTreeEquation()->allouer<NodeRefVariable>(nomVariable);
}

#endif /* PARSEUR_REFVARIABLE_CPP */




