#ifndef A2837407_B466_49AE_8A29_4BFC0A5D0461
#define A2837407_B466_49AE_8A29_4BFC0A5D0461

#include "Compilateur/AST/Noeuds/Instruction.h"
#include "Compilateur/AST/Registre/Pile/ReturnContextCompilation.h"
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
    std::shared_ptr<ReturnContextCompilation> _returnContextCompilation; 

public:
    NoeudDeclarationFonction(std::shared_ptr<LLVMBackend> backend, std::shared_ptr<RegistreVariable> registreVariable, std::string nom, TokenType typeRetour, std::shared_ptr<ReturnContextCompilation> returnContextCompilation);
    
    ~NoeudDeclarationFonction() = default;

    llvm::Value* genCode() override;
};

#endif /* A2837407_B466_49AE_8A29_4BFC0A5D0461 */
