#ifndef A6143133_47EE_4E86_9715_0D00D76F1DE5
#define A6143133_47EE_4E86_9715_0D00D76F1DE5
#include "Compilateur/AST/Noeuds/Instruction.h"
#include "Compilateur/AST/Registre/RegistreFonction.h"
#include "Compilateur/LLVM/LLVMBackend.h"
#include "Compilateur/Lexer/Lexer.h"
#include <memory>

class NoeudAppelFonction : public Instruction
{
private: 
    Token _nomFonction;
    std::shared_ptr<RegistreFonction> _registreFonction;
    std::shared_ptr<LLVMBackend> _backend;

public:
    NoeudAppelFonction(Token nomFonction, std::shared_ptr<RegistreFonction> registreFonction, std::shared_ptr<LLVMBackend> backend);
    ~NoeudAppelFonction();
    llvm::Value* genCode() override;
};

#endif /* A6143133_47EE_4E86_9715_0D00D76F1DE5 */
