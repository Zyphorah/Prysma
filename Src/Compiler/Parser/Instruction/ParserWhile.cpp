#ifndef PARSEUR_WHILE_CPP
#define PARSEUR_WHILE_CPP

#include "Compiler/Instruction/ParserWhile.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Nodes/Interfaces/IInstruction.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Nodes/NodeInstruction.h"
#include "Compiler/AST/Registry/ContextParser.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include <vector>


ParserWhile::ParserWhile(ContextParser& contextParser) 
    : _contextParser(contextParser)
{}

ParserWhile::~ParserWhile() = default;

INode* ParserWhile::parser(std::vector<Token>& tokens, int& index)
{
    consommer(tokens,index,TOKEN_TANT_QUE,"Error, token attendu 'while' ");

    consommer(tokens,index,TOKEN_PAREN_OUVERTE,"Error, le token n'est pas '('! ");
    
    INode* condition = _contextParser.getBuilderTreeEquation()->construire(tokens, index);

    consommer(tokens,index,TOKEN_PAREN_FERMEE,"Error, le token n'est pas ')'! ");

    IInstruction* nodeBlocWhile = _contextParser.getBuilderTreeInstruction()->allouer<NodeInstruction>();
    consommer(tokens,index,TOKEN_ACCOLADE_OUVERTE, "Error, le token n'est pas '{'");
    consommerChildBody(tokens,index,nodeBlocWhile,_contextParser.getBuilderTreeInstruction(),TOKEN_ACCOLADE_FERMEE);
    consommer(tokens,index,TOKEN_ACCOLADE_FERMEE,"Error, le token n'est pas '}'");


    IInstruction* nodeBlocEndWhile = _contextParser.getBuilderTreeInstruction()->allouer<NodeInstruction>();

    IInstruction* nodeWhile = _contextParser.getBuilderTreeInstruction()->allouer<NodeWhile>(condition, nodeBlocWhile, nodeBlocEndWhile);

    return nodeWhile;
}

#endif /* PARSEUR_WHILE_CPP */




