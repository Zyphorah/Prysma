#ifndef A2837407_B466_49AE_8A29_4BFC0A5D0461
#define A2837407_B466_49AE_8A29_4BFC0A5D0461

#include "Compilateur/AST/Noeuds/Instruction.h"
#include "Compilateur/Lexer/TokenType.h"
#include <string>
#include <memory>

class LLVMBackend;
class RegistreVariable;

class NoeudDeclarationFonction : public Instruction
{
private:
    std::shared_ptr<LLVMBackend> _backend;
    std::shared_ptr<RegistreVariable> _registreVariable;
    std::string _nom;
    TokenType _typeRetourToken;

public:
    NoeudDeclarationFonction(std::shared_ptr<LLVMBackend> backend, std::shared_ptr<RegistreVariable> registreVariable, std::string nom, TokenType typeRetour);
    
    ~NoeudDeclarationFonction() = default;

    llvm::Value* genCode() override;

    [[nodiscard]] std::string getNom() const { return _nom; }
    [[nodiscard]] TokenType getTypeRetourToken() const { return _typeRetourToken; }
};

#endif /* A2837407_B466_49AE_8A29_4BFC0A5D0461 */
