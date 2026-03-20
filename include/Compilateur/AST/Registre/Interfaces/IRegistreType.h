#ifndef DD937933_DED1_40A8_819C_360C96A8F41C
#define DD937933_DED1_40A8_819C_360C96A8F41C

#include "Compilateur/AST/Registre/RegistreGeneric.h"
#include "Compilateur/Lexer/TokenType.h"
#include <llvm/IR/Type.h>

class IRegistreType : public RegistreGeneric<TokenType, llvm::Type*> {
public:
    IRegistreType() = default;
    IRegistreType(const IRegistreType&) = delete;
    auto operator=(const IRegistreType&) -> IRegistreType& = delete;
    IRegistreType(IRegistreType&&) = delete;
    auto operator=(IRegistreType&&) -> IRegistreType& = delete;
     ~IRegistreType() override = default;
};

#endif /* DD937933_DED1_40A8_819C_360C96A8F41C */
