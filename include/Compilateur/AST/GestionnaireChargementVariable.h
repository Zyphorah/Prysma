#ifndef GESTIONNAIRE_CHARGEMENT_VARIABLE_H
#define GESTIONNAIRE_CHARGEMENT_VARIABLE_H

#include "Compilateur/LLVM/LLVMBackend.h"
#include <llvm/IR/Value.h>
#include <llvm/IR/Type.h>
#include <string>
#include <memory>


class GestionnaireChargementVariable
{
private:
    std::shared_ptr<LLVMBackend> _backend;

public:
    GestionnaireChargementVariable(std::shared_ptr<LLVMBackend> backend);

    llvm::Value* chargerVariable(llvm::Value* adresseMemoire, const std::string& nomVariable);

private:
    llvm::Type* determinerType(llvm::Value* adresseMemoire);
};

#endif /* GESTIONNAIRE_CHARGEMENT_VARIABLE_H */
