#ifndef B18E8AB9_A311_4560_9FDD_5E2D3FAC0F14
#define B18E8AB9_A311_4560_9FDD_5E2D3FAC0F14

#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/LLVM/LLVMBackend.h"
#include <llvm/IR/Instructions.h>
#include <memory>

class NoeudDeclaration : public INoeud
{
private:
    std::shared_ptr<LLVMBackend> _backend;
    std::string _nom;
    llvm::Type* _type;
    llvm::Value* _arraySize;
    llvm::Value* _valeur;

    llvm::AllocaInst* allocation();
    llvm::AllocaInst* initialisation(llvm::AllocaInst* allocaInst);

public:
    NoeudDeclaration(std::shared_ptr<LLVMBackend> backend, const std::string& nom, llvm::Type* type, llvm::Value* valeur);
    ~NoeudDeclaration();

    llvm::Value* genCode() override;
};

#endif /* B18E8AB9_A311_4560_9FDD_5E2D3FAC0F14 */
