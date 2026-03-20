#ifndef F5F52A3C_374A_47C1_868F_9E81A1FED8AD
#define F5F52A3C_374A_47C1_868F_9E81A1FED8AD

#include "Compilateur/AST/Registre/RegistreGeneric.h"
#include "Compilateur/AST/Noeuds/Expression/IExpressionMoteur.h"
#include "Compilateur/Lexer/TokenType.h"

class RegistreExpression : public RegistreGeneric<TokenType, IExpressionMoteur*>
{
public:
    RegistreExpression() = default;
    RegistreExpression(const RegistreExpression&) = delete;
    auto operator=(const RegistreExpression&) -> RegistreExpression& = delete;
    RegistreExpression(RegistreExpression&&) = delete;
    auto operator=(RegistreExpression&&) -> RegistreExpression& = delete;
    ~RegistreExpression() override = default;
};

#endif /* F5F52A3C_374A_47C1_868F_9E81A1FED8AD */