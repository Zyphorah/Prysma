#ifndef EXPRESSION_APPELCENTRAL_H
#define EXPRESSION_APPELCENTRAL_H

#include "Compiler/AST/Nodes/Expression/IExpressionEngine.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/ContextExpression.h"
#include "Compiler/Lexer/Lexer.h"
#include <vector>

class ExpressionCallCentral : public IExpressionEngine
{
private:
    ContextExpression& _contexteExpression;

public:
    explicit ExpressionCallCentral(ContextExpression& contexteExpression);
    ~ExpressionCallCentral() override;

    ExpressionCallCentral(const ExpressionCallCentral&) = delete;
    auto operator=(const ExpressionCallCentral&) -> ExpressionCallCentral& = delete;

    ExpressionCallCentral(ExpressionCallCentral&&) = delete;
    auto operator=(ExpressionCallCentral&&) -> ExpressionCallCentral& = delete;

    auto construire(std::vector<Token>& equation) -> INode* override;
};

#endif /* EXPRESSION_APPELCENTRAL_H */