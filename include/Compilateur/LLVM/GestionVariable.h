#ifndef D5B94044_819C_4C15_B528_AAE97CBCB264
#define D5B94044_819C_4C15_B528_AAE97CBCB264

#include "Compilateur/AST/Registre/ContextGenCode.h"
#include <llvm-18/llvm/IR/Instructions.h>
#include <llvm-18/llvm/IR/Value.h>
#include <memory>

class GestionVariable
{
    private:
        ContextGenCode* _contextGenCode;

        llvm::Type* extraireTypeDonnee(llvm::Value* adresseMemoire);

    public:
        GestionVariable(ContextGenCode* contextGenCode);
        ~GestionVariable();

        llvm::Value* chargerVariable(const std::string& nomVariable);

        llvm::AllocaInst* allouerVariable(llvm::Type* type, const std::string& nomVariable);

        void enregistrerVariable(const std::string& nomVariable, llvm::AllocaInst* allocaInst);

        void stockerVariable(llvm::Value* valeur, llvm::AllocaInst* allocaInst);

        void affecterVariable(llvm::AllocaInst* allocaInst, llvm::Value* valeur);
};

#endif /* D5B94044_819C_4C15_B528_AAE97CBCB264 */
