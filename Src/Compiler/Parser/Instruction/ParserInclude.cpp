#ifndef PARSEUR_INCLUDE_CPP
#define PARSEUR_INCLUDE_CPP

#include "Compiler/Instruction/ParserInclude.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/ContextParser.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include <vector>


ParserInclude::ParserInclude(ContextParser& contextParser) 
    : _contextParser(contextParser)
{}

ParserInclude::~ParserInclude()
= default;

// Exemple : include "path"
auto ParserInclude::parser(std::vector<Token>& tokens, int& index) -> INode*
{
    consommer(tokens,index,TOKEN_INCLUDE, "Error : Instruction include doit commencer par le mot-clé 'include'");
    consommer(tokens,index,TOKEN_GUILLEMET, "Error : Instruction include doit être suivie d'une chaîne de caractères input guillemets");
    Token tokenPath = consommer(tokens,index,TOKEN_IDENTIFIANT, "Error : Instruction include doit contenir un chemin de fichier");
    consommer(tokens,index,TOKEN_GUILLEMET, "Error : Instruction include doit être suivie d'une chaîne de caractères input guillemets");
    consommer(tokens,index,TOKEN_POINT_VIRGULE, "Error : Instruction include doit se terminer par un point-virgule");

    return _contextParser.getBuilderTreeEquation()->allouer<NodeInclude>(tokenPath.value);
}

#endif /* PARSEUR_INCLUDE_CPP */




