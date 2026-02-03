#ifndef DC6C556F_6309_4430_98EB_911BFF855216
#define DC6C556F_6309_4430_98EB_911BFF855216

#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/LLVM/LLVMBackend.h"
#include <llvm-18/llvm/IR/Value.h>
#include <memory>

class NoeudReturn : public INoeud
{
private:
    std::shared_ptr<LLVMBackend> _backend;
    std::shared_ptr<INoeud> _valeurRetour;
    llvm::Type* _typeRetour;

public:
    NoeudReturn(std::shared_ptr<LLVMBackend> backend, std::shared_ptr<INoeud> valeurRetour);
    ~NoeudReturn() = default;

    llvm::Value* genCode() override;
};


#endif /* DC6C556F_6309_4430_98EB_911BFF855216 */
