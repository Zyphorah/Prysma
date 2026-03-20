#ifndef EXPRESSION_APPELCENTRAL_H
#define EXPRESSION_APPELCENTRAL_H

#include "Compilateur/AST/Noeuds/Expression/IExpressionMoteur.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Registre/ContexteExpression.h"
#include "Compilateur/Lexer/Lexer.h"
#include <vector>

class ExpressionAppelCentral : public IExpressionMoteur
{
private:
    ContexteExpression& _contexteExpression;

public:
    explicit ExpressionAppelCentral(ContexteExpression& contexteExpression);
    ~ExpressionAppelCentral() override;

    ExpressionAppelCentral(const ExpressionAppelCentral&) = delete;
    auto operator=(const ExpressionAppelCentral&) -> ExpressionAppelCentral& = delete;

    ExpressionAppelCentral(ExpressionAppelCentral&&) = delete;
    auto operator=(ExpressionAppelCentral&&) -> ExpressionAppelCentral& = delete;

    auto construire(std::vector<Token>& equation) -> INoeud* override;
};

#endif /* EXPRESSION_APPELCENTRAL_H */