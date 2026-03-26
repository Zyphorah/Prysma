#ifndef DD335087_6EDE_4036_872C_8BD586E26252
#define DD335087_6EDE_4036_872C_8BD586E26252

#include "Compiler/AST/Registry/RegistryGeneric.h"
#include "Interfaces/IRegistryType.h"
#include "Compiler/Lexer/TokenType.h"
#include <llvm/IR/Type.h>
#include <string>

class RegistryType : public IRegistryType
{

public:
    RegistryType() = default;
    ~RegistryType() override = default;
    RegistryType(const RegistryType&) = delete;
    auto operator=(const RegistryType&) -> RegistryType& = delete;
    RegistryType(RegistryType&&) = delete;
    auto operator=(RegistryType&&) -> RegistryType& = delete;

protected:
    
    [[nodiscard]] auto generateErrorMessage(const TokenType& key) const -> std::string override {
        return RegistryGeneric::generateErrorMessage(key);
    }
};

#endif /* DD335087_6EDE_4036_872C_8BD586E26252 */
