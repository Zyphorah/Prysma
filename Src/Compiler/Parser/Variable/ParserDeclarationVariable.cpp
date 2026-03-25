#ifndef PARSEUR_DECLARATIONVARIABLE_CPP
#define PARSEUR_DECLARATIONVARIABLE_CPP

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

auto ParserDeclarationVariable::parser(std::vector<Token>& tokens, int& index) -> INode*
{
    consommer(tokens, index, TOKEN_DEC, "Error : type attendu 'dec");
    
    // Utiliser le ParserType pour analyser le type (simple ou array)
    IType* type = _contextParser.getParserType()->parser(tokens, index);
    
    Token nomToken = consommer(tokens, index, TOKEN_IDENTIFIANT, "Error : nom de variable attendu");
    std::string nomVariable = nomToken.value;
    
    consommer(tokens, index, TOKEN_EGAL, "Error : '=' attendu après le nom de variable");
    
    INode* expression = _contextParser.getBuilderTreeEquation()->construire(tokens, index);
    
    consommer(tokens, index, TOKEN_POINT_VIRGULE, "Error : ';' attendu à la fin de la déclaration");

    return _contextParser.getBuilderTreeEquation()->allouer<NodeDeclarationVariable>(
        Token{},
        nomVariable,
        type,
        expression
    );
}
#endif /* PARSEUR_DECLARATIONVARIABLE_CPP */




