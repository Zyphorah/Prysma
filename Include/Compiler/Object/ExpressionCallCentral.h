#ifndef EXPRESSION_CALLCENTRAL_H
#define EXPRESSION_CALLCENTRAL_H

#include "Compiler/AST/Nodes/Expression/IExpressionEngine.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/ContextExpression.h"
#include "Compiler/Lexer/Lexer.h"
#include <vector>

class ExpressionCallCentral : public IExpressionEngine
{
private:
    ContextExpression& _context;

public:
    explicit ExpressionCallCentral(ContextExpression& expressionContext);
    ~ExpressionCallCentral() override;

    ExpressionCallCentral(const ExpressionCallCentral&) = delete;
    auto operator=(const ExpressionCallCentral&) -> ExpressionCallCentral& = delete;

    ExpressionCallCentral(ExpressionCallCentral&&) = delete;
    auto operator=(ExpressionCallCentral&&) -> ExpressionCallCentral& = delete;

    auto build(std::vector<Token>& equation) -> INode* override;
};

#endif /* EXPRESSION_CALLCENTRAL_H */