#ifndef C2F85AFD_7B01_4D87_8D01_5C59514D2B2E
#define C2F85AFD_7B01_4D87_8D01_5C59514D2B2E

#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/Lexer/Lexer.h"
#include <vector>

class IExpression
{
public:
    virtual ~IExpression() = default;
    IExpression(const IExpression&) = delete;
    auto operator=(const IExpression&) -> IExpression& = delete;
    IExpression(IExpression&&) = delete;
    auto operator=(IExpression&&) -> IExpression& = delete;
    virtual auto construire(std::vector<Token>& equation) -> INoeud* = 0;
};

#endif /* C2F85AFD_7B01_4D87_8D01_5C59514D2B2E */