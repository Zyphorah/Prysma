#ifndef E73A6A09_CFBA_4637_B0BE_23036BCEE579
#define E73A6A09_CFBA_4637_B0BE_23036BCEE579

#include "Compilateur/AST/Registre/RegistreGeneric.h"
#include "Compilateur/AST/Noeuds/StrategieEquation/IStrategieEquation.h"
#include "Compilateur/Lexer/TokenType.h"

class RegistreStrategieEquation : public RegistreGeneric<TokenType, IStrategieEquation*> {
public:
    RegistreStrategieEquation() = default;
    ~RegistreStrategieEquation() override = default;
};


#endif /* E73A6A09_CFBA_4637_B0BE_23036BCEE579 */
