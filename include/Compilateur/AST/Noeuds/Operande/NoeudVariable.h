#ifndef C9126446_60DE_4831_AEE0_655AF47AE40D
#define C9126446_60DE_4831_AEE0_655AF47AE40D

#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Registre/Pile/RegistreVariable.h" 
#include "Compilateur/LLVM/LLVMBackend.h"            
#include <memory>
#include <string>
#include <llvm/IR/Value.h>

class NoeudVariable : public INoeud
{
private:

    std::shared_ptr<LLVMBackend> _backend;
    std::shared_ptr<RegistreVariable> _registre;
    std::string _nomVariable;

public:

    NoeudVariable(std::shared_ptr<LLVMBackend> backend, std::shared_ptr<RegistreVariable> registre, const std::string& nomVariable);
    
    ~NoeudVariable();

    llvm::Value* genCode() override;
};

#endif /* C9126446_60DE_4831_AEE0_655AF47AE40D */