#ifndef PARSEUR_APPELCENTRAL_H
#define PARSEUR_APPELCENTRAL_H

#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/ContextParser.h"
#include "Compiler/Parser/Interfaces/IParser.h"
#include "Compiler/Lexer/Lexer.h"
#include <vector>

class ParserCallCentral : public IParser
{
private:
    ContextParser& _contextParser;

public:
    explicit ParserCallCentral(ContextParser& contextParser);
    ~ParserCallCentral() override;

    ParserCallCentral(const ParserCallCentral&) = delete;
    auto operator=(const ParserCallCentral&) -> ParserCallCentral& = delete;
    ParserCallCentral(ParserCallCentral&&) = delete;
    auto operator=(ParserCallCentral&&) -> ParserCallCentral& = delete;
    
    auto parser(std::vector<Token>& tokens, int& index) -> INode* override;
};

#endif /* PARSEUR_APPELCENTRAL_H */