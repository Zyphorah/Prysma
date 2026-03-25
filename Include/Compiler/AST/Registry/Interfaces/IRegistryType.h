#ifndef DD937933_DED1_40A8_819C_360C96A8F41C
#define DD937933_DED1_40A8_819C_360C96A8F41C

#include "Compiler/AST/Registry/RegistryGeneric.h"
#include "Compiler/Lexer/TokenType.h"
#include <llvm/IR/Type.h>

class IRegistryType : public RegistryGeneric<TokenType, llvm::Type*> {
public:
    IRegistryType() = default;
    IRegistryType(const IRegistryType&) = delete;
    auto operator=(const IRegistryType&) -> IRegistryType& = delete;
    IRegistryType(IRegistryType&&) = delete;
    auto operator=(IRegistryType&&) -> IRegistryType& = delete;
     ~IRegistryType() override = default;
};

#endif /* DD937933_DED1_40A8_819C_360C96A8F41C */
