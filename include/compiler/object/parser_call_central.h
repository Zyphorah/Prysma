#ifndef PARSER_CALLCENTRAL_H
#define PARSER_CALLCENTRAL_H

#include "compiler/ast/nodes/interfaces/i_node.h"
#include "compiler/ast/registry/context_parser.h"
#include "compiler/parser/interfaces/i_parser.h"
#include "compiler/lexer/lexer.h"
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
    
    auto parse(std::vector<Token>& tokens, std::size_t index) -> INode* override;
};

#endif /* PARSER_CALLCENTRAL_H */