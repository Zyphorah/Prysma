#ifndef DD335087_6EDE_4036_872C_8BD586E26252
#define DD335087_6EDE_4036_872C_8BD586E26252

#include "Compilateur/AST/Registre/RegistreGeneric.h"
#include "Interfaces/IRegistreType.h"
#include "Compilateur/Lexer/TokenType.h"
#include <llvm/IR/Type.h>
#include <string>

class RegistreType : public IRegistreType
{

public:
    RegistreType() = default;
    ~RegistreType() override = default;
    RegistreType(const RegistreType&) = delete;
    auto operator=(const RegistreType&) -> RegistreType& = delete;
    RegistreType(RegistreType&&) = delete;
    auto operator=(RegistreType&&) -> RegistreType& = delete;

protected:
    
    [[nodiscard]] auto genererMessageErreur(const TokenType& cle) const -> std::string override {
        return RegistreGeneric::genererMessageErreur(cle);
    }
};

#endif /* DD335087_6EDE_4036_872C_8BD586E26252 */
