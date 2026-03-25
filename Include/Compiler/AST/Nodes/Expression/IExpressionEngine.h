#ifndef C2F85AFD_7B01_4D87_8D01_5C59514D2B2E
#define C2F85AFD_7B01_4D87_8D01_5C59514D2B2E

#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/Lexer/Lexer.h"
#include <vector>

class IExpressionEngine
{
public:
    IExpressionEngine() = default;
    IExpressionEngine(const IExpressionEngine&) = delete;
    auto operator=(const IExpressionEngine&) -> IExpressionEngine& = delete;
    IExpressionEngine(IExpressionEngine&&) = delete;
    auto operator=(IExpressionEngine&&) -> IExpressionEngine& = delete;

    virtual ~IExpressionEngine() = default;
    virtual auto construire(std::vector<Token>& equation) -> INode* = 0;
};

#endif /* C2F85AFD_7B01_4D87_8D01_5C59514D2B2E */