#ifndef PARSER_WHILE_CPP
#define PARSER_WHILE_CPP

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

INode* ParserWhile::parse(std::vector<Token>& tokens, int& index)
{
    consume(tokens, index, TOKEN_WHILE, "Error, expected token 'while' ");

    consume(tokens, index, TOKEN_PAREN_OPEN, "Error, token is not '('! ");
    
    INode* condition = _contextParser.getBuilderTreeEquation()->build(tokens, index);

    consume(tokens, index, TOKEN_PAREN_CLOSE, "Error, token is not ')'! ");

    IInstruction* nodeBlockWhile = _contextParser.getBuilderTreeInstruction()->allocate<NodeInstruction>();
    consume(tokens, index, TOKEN_BRACE_OPEN, "Error, token is not '{'");
    consumeChildBody(tokens, index, nodeBlockWhile, _contextParser.getBuilderTreeInstruction(), TOKEN_BRACE_CLOSE);
    consume(tokens, index, TOKEN_BRACE_CLOSE, "Error, token is not '}'");

    IInstruction* nodeBlockEndWhile = _contextParser.getBuilderTreeInstruction()->allocate<NodeInstruction>();

    IInstruction* nodeWhile = _contextParser.getBuilderTreeInstruction()->allocate<NodeWhile>(condition, nodeBlockWhile, nodeBlockEndWhile);

    return nodeWhile;
}

#endif /* PARSER_WHILE_CPP */




