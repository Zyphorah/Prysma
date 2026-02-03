#ifndef B18E8AB9_A311_4560_9FDD_5E2D3FAC0F14
#define B18E8AB9_A311_4560_9FDD_5E2D3FAC0F14

#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Registre/RegistreType.h"
#include "Compilateur/LLVM/LLVMBackend.h"
#include "Compilateur/Lexer/TokenType.h"
#include <llvm/IR/Instructions.h>
#include <memory>

class RegistreVariable;

class NoeudDeclaration : public INoeud
{
private:
    std::shared_ptr<LLVMBackend> _backend;
    std::shared_ptr<RegistreVariable> _registreVariable;
    std::string _nom;
    llvm::Value* _arraySize;
    std::shared_ptr<INoeud> _expression;
    std::shared_ptr<RegistreType> _registreType; 
    TokenType _token; 

    llvm::AllocaInst* allocation();
    llvm::AllocaInst* initialisation(llvm::AllocaInst* allocaInst, llvm::Value* valeur);

public:
    NoeudDeclaration(std::shared_ptr<LLVMBackend> backend, std::shared_ptr<RegistreVariable> registreVariable, const std::string& nom, std::shared_ptr<INoeud> expression, std::shared_ptr<RegistreType> registreType, TokenType token);
    ~NoeudDeclaration();

    llvm::Value* genCode() override;
};

#endif /* B18E8AB9_A311_4560_9FDD_5E2D3FAC0F14 */
