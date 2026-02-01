#ifndef F529C0E5_B051_4DF3_9DB0_8987D960AAF6
#define F529C0E5_B051_4DF3_9DB0_8987D960AAF6

#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/LLVM/LLVMBackend.h"
#include <llvm/IR/Instructions.h>
#include <memory>

class NoeudAffectation : public INoeud
{
private:
    std::shared_ptr<LLVMBackend> _backend;
    std::string _nom;
    llvm::AllocaInst* _variableAssigner;
    std::shared_ptr<INoeud> _expression;

    void assignation(llvm::AllocaInst* allocaInst, llvm::Value* valeur);

public:
    NoeudAffectation(std::shared_ptr<LLVMBackend> backend, const std::string& nom, llvm::AllocaInst* variableAssigner, std::shared_ptr<INoeud> expression);
    ~NoeudAffectation();

    llvm::Value* genCode() override;
};

#endif /* F529C0E5_B051_4DF3_9DB0_8987D960AAF6 */
