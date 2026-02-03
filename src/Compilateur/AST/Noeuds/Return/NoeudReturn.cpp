#include "Compilateur/AST/Noeuds/Return/NoeudReturn.h"
#include "Compilateur/LLVM/LLVMBackend.h"
#include <llvm-18/llvm/IR/Instructions.h>
#include <utility>

NoeudReturn::NoeudReturn(std::shared_ptr<LLVMBackend> backend, std::shared_ptr<INoeud> valeurRetour)
    : _backend(std::move(backend)), _valeurRetour(std::move(valeurRetour)), _typeRetour(nullptr)
{
}

llvm::Value* NoeudReturn::genCode()
{
    llvm::Value* valeur = _valeurRetour->genCode();
    if (valeur == nullptr) {
        throw std::runtime_error("Erreur : impossible d'évaluer l'expression de retour");
    }

    // Seulement effectuer le cast si un type de retour est défini
    if (_typeRetour != nullptr && valeur->getType() != _typeRetour) {
        valeur = _backend->getBuilder().CreateFPToSI(valeur, _typeRetour);
    }
    
    _backend->getBuilder().CreateRet(valeur);
    return valeur;
}
