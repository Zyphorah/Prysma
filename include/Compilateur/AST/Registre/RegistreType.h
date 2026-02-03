#ifndef DD335087_6EDE_4036_872C_8BD586E26252
#define DD335087_6EDE_4036_872C_8BD586E26252

#include "RegistreGeneric.h"
#include "Compilateur/Lexer/TokenType.h"
#include <llvm/IR/Type.h>

class RegistreType : public RegistreGeneric<TokenType,llvm::Type*>
{

public:
    RegistreType() = default;
    virtual ~RegistreType() = default;

    llvm::Type* getType(TokenType typeToken) {
        return recuperer(typeToken);
    }

  
    [[nodiscard]] bool typeExists(TokenType typeToken) const {
        return existe(typeToken);
    }

protected:
    
    [[nodiscard]] std::string genererMessageErreur(TokenType cle) const  {
        return RegistreGeneric::genererMessageErreur(cle);
    }
};

#endif /* DD335087_6EDE_4036_872C_8BD586E26252 */
