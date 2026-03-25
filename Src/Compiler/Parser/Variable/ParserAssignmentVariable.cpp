#ifndef PARSEUR_AFFECTATIONVARIABLE_CPP
#define PARSEUR_AFFECTATIONVARIABLE_CPP

#include "Compiler/Variable/ParserAssignmentVariable.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/ContextParser.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include <cstddef>
#include <string>
#include <vector>


ParserAssignmentVariable::ParserAssignmentVariable(ContextParser& contextParser) 
    : _contextParser(contextParser)
{}

ParserAssignmentVariable::~ParserAssignmentVariable()
= default;

auto ParserAssignmentVariable::parser(std::vector<Token>& tokens, int& index) -> INode*
{
    consommer(tokens, index, TOKEN_AFF, "Error : 'aff' attendu");
    Token nomToken = consommer(tokens, index, TOKEN_IDENTIFIANT, "Error : nom variable attendu");
    std::string nomVariable = nomToken.value;

    if (tokens[static_cast<size_t>(index)].type == TOKEN_POINT) {
        consommer(tokens, index, TOKEN_POINT, "Error '.'");
        Token attributeToken = consommer(tokens, index, TOKEN_IDENTIFIANT, "Error : attribute attendu");
        nomVariable += "." + attributeToken.value;
        nomToken.value = nomVariable;
    }

    INode* expressionIndex = nullptr;

    if (tokens[static_cast<size_t>(index)].type == TOKEN_CROCHET_OUVERT) {
 
        consommer(tokens, index, TOKEN_CROCHET_OUVERT, "Error '['");
        expressionIndex = _contextParser.getBuilderTreeEquation()->construire(tokens, index);
        consommer(tokens, index, TOKEN_CROCHET_FERME, "Error : ']' attendu après l'index");
    }

    consommer(tokens, index, TOKEN_EGAL, "Error : '=' attendu");
    INode* expression = _contextParser.getBuilderTreeEquation()->construire(tokens, index);
    consommer(tokens, index, TOKEN_POINT_VIRGULE, "Error : ';' attendu");

    if (expressionIndex != nullptr) {
        return _contextParser.getBuilderTreeEquation()->allouer<NodeAssignmentArray>(nomVariable, expressionIndex, expression, nomToken);
    }
    
    return _contextParser.getBuilderTreeEquation()->allouer<NodeAssignmentVariable>(nomVariable, expression, nomToken);
}

#endif /* PARSEUR_AFFECTATIONVARIABLE_CPP */




