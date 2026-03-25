#ifndef PARSEUR_DELETE_CPP
#define PARSEUR_DELETE_CPP

#include "Compiler/Instruction/ParserDelete.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/ContextParser.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include <vector>


ParserDelete::ParserDelete(ContextParser& contextParser) 
    : _contextParser(contextParser)
{}

ParserDelete::~ParserDelete()
= default;

// Exemple : delete variableNom;
auto ParserDelete::parser(std::vector<Token>& tokens, int& index) -> INode*
{
    consommer(tokens, index, TOKEN_DELETE, "Attendu 'delete' au début de l'instruction delete.");
    Token identifiantToken = consommer(tokens, index, TOKEN_IDENTIFIANT, "Attendu un identifiant après 'delete'.");
    consommer(tokens,index, TOKEN_POINT_VIRGULE, "Attendu ';' après l'identifiant dans l'instruction delete.");

    return _contextParser.getBuilderTreeInstruction()->allouer<NodeDelete>(identifiantToken);
}

#endif /* PARSEUR_DELETE_CPP */




