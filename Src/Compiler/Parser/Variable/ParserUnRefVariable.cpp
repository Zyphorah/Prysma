#ifndef PARSEUR_UNREFVARIABLE_CPP
#define PARSEUR_UNREFVARIABLE_CPP

#include "Compiler/Variable/ParserUnRefVariable.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/ContextParser.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include <string>
#include <vector>


ParserUnRefVariable::ParserUnRefVariable(ContextParser& contextParser) 
    : _contextParser(contextParser)
{}

ParserUnRefVariable::~ParserUnRefVariable()
= default;

// Exemple unref variable
INode* ParserUnRefVariable::parser(std::vector<Token>& tokens, int& index) 
{
    consommer(tokens, index, TOKEN_UNREF, "Error : 'unref' attendu");
    
    Token nomToken = consommer(tokens, index, TOKEN_IDENTIFIANT, "Error : nom de variable attendu après 'unref'");
    std::string nomVariable = nomToken.value;
    
    return _contextParser.getBuilderTreeEquation()->allouer<NodeUnRefVariable>(nomVariable);
}

#endif /* PARSEUR_UNREFVARIABLE_CPP */




