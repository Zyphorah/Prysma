#ifndef F5F52A3C_374A_47C1_868F_9E81A1FED8AD
#define F5F52A3C_374A_47C1_868F_9E81A1FED8AD

#include "Compiler/AST/Registry/RegistryGeneric.h"
#include "Compiler/AST/Nodes/Expression/IExpressionEngine.h"
#include "Compiler/Lexer/TokenType.h"

class RegistryExpression : public RegistryGeneric<TokenType, IExpressionEngine*>
{
public:
    RegistryExpression() = default;
    RegistryExpression(const RegistryExpression&) = delete;
    auto operator=(const RegistryExpression&) -> RegistryExpression& = delete;
    RegistryExpression(RegistryExpression&&) = delete;
    auto operator=(RegistryExpression&&) -> RegistryExpression& = delete;
    ~RegistryExpression() override = default;
};

#endif /* F5F52A3C_374A_47C1_868F_9E81A1FED8AD */