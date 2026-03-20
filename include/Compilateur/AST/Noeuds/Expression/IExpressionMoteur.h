#ifndef C2F85AFD_7B01_4D87_8D01_5C59514D2B2E
#define C2F85AFD_7B01_4D87_8D01_5C59514D2B2E

#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/Lexer/Lexer.h"
#include <vector>

class IExpressionMoteur
{
public:
    IExpressionMoteur() = default;
    IExpressionMoteur(const IExpressionMoteur&) = delete;
    auto operator=(const IExpressionMoteur&) -> IExpressionMoteur& = delete;
    IExpressionMoteur(IExpressionMoteur&&) = delete;
    auto operator=(IExpressionMoteur&&) -> IExpressionMoteur& = delete;

    virtual ~IExpressionMoteur() = default;
    virtual auto construire(std::vector<Token>& equation) -> INoeud* = 0;
};

#endif /* C2F85AFD_7B01_4D87_8D01_5C59514D2B2E */