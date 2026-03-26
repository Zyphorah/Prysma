#ifndef PARSER_CALLCENTRAL_CPP
#define PARSER_CALLCENTRAL_CPP

#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/Function/ParserCallFunction.h"
#include "Compiler/Lexer/Lexer.h"
#include "Compiler/Lexer/TokenType.h"
#include "Compiler/Object/ParserCallObject.h"

#include "Compiler/Object/ParserCallCentral.h"

#include "Compiler/AST/Registry/ContextParser.h"
#include <cstddef>
#include <vector>

ParserCallCentral::ParserCallCentral(ContextParser& contextParser)
    : _contextParser(contextParser)
{}

ParserCallCentral::~ParserCallCentral()
= default;

INode* ParserCallCentral::parse(std::vector<Token>& tokens, int& index)
{
    const auto currentIndex = static_cast<size_t>(index);
    const bool callObject = currentIndex + 2 < tokens.size()
        && tokens[currentIndex].type == TOKEN_CALL
        && tokens[currentIndex + 1].type == TOKEN_IDENTIFIER
        && tokens[currentIndex + 2].type == TOKEN_DOT;

    if (callObject) {
        ParserCallObject parserObject(_contextParser);
        return parserObject.parse(tokens, index);
    }

    ParserCallFunction parserFunction(_contextParser);
    return parserFunction.parse(tokens, index);
}

#endif /* PARSER_CALLCENTRAL_CPP */