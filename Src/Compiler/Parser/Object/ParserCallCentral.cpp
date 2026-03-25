#ifndef PARSEUR_APPELCENTRAL_CPP
#define PARSEUR_APPELCENTRAL_CPP

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

INode* ParserCallCentral::parser(std::vector<Token>& tokens, int& index)
{
    const auto indexCourant = static_cast<size_t>(index);
    const bool callObject = indexCourant + 2 < tokens.size()
        && tokens[indexCourant].type == TOKEN_CALL
        && tokens[indexCourant + 1].type == TOKEN_IDENTIFIANT
        && tokens[indexCourant + 2].type == TOKEN_POINT;

    if (callObject) {
        ParserCallObject parserObject(_contextParser);
        return parserObject.parser(tokens, index);
    }

    ParserCallFunction parserFunction(_contextParser);
    return parserFunction.parser(tokens, index);
}

#endif /* PARSEUR_APPELCENTRAL_CPP */