#include "Compilateur/AST/Noeuds/Variable/NoeudAffectation.h"
#include "Compilateur/AST/Registre/RegistreVariable.h"
#include "Compilateur/LLVM/LLVMBackend.h"

#include <llvm/IR/Instructions.h>
#include <llvm/IR/Value.h>
#include <memory>
#include <stdexcept>
#include <utility>

NoeudAffectation::NoeudAffectation(std::shared_ptr<LLVMBackend> backend, const std::string& nom, std::shared_ptr<INoeud> expression, std::shared_ptr<RegistreVariable> registreVariable,Token token)
    : _backend(std::move(backend)), _nom(nom),  _expression(std::move(expression)), _registreVariable(std::move(registreVariable)), _token(std::move(token))
{
}

NoeudAffectation::~NoeudAffectation()
{
}

llvm::AllocaInst* NoeudAffectation::recupererVariable()
{
// Récupérer la variable existante du registre
    llvm::AllocaInst* variableExistante = nullptr;
    if (_registreVariable != nullptr) {
        try {
            llvm::Value* valeur = _registreVariable->recupererVariables(_token);
            variableExistante = llvm::dyn_cast<llvm::AllocaInst>(valeur);
        } catch (const std::exception& e) {
            // La variable n'existe pas
            throw std::runtime_error(std::string("Erreur : la variable '") + _nom + "' n'existe pas. Vous devez d'abord la déclarer avec 'dec type nom = valeur;'");
        }
    }
    return variableExistante;
}

llvm::Value* NoeudAffectation::genCode()
{

    // Évaluer l'expression et assigner immédiatement
    llvm::Value* valeur = nullptr;
    if (_expression != nullptr) {
        valeur = _expression->genCode();
    }
    assignation(recupererVariable(), valeur);
    
    return nullptr;
}

void NoeudAffectation::assignation(llvm::AllocaInst* allocaInst, llvm::Value* valeur)
{
    if (valeur != nullptr && allocaInst != nullptr)
    {
        // Si la valeur est une AllocaInst (variable), charger sa valeur
        if (auto* sourceAlloca = llvm::dyn_cast<llvm::AllocaInst>(valeur)) {
            llvm::Value* loadedValue = _backend->getBuilder().CreateLoad(sourceAlloca->getAllocatedType(), sourceAlloca);
            _backend->getBuilder().CreateStore(loadedValue, allocaInst);
        } else {
            // Sinon, assigner directement la valeur
            _backend->getBuilder().CreateStore(valeur, allocaInst);
        }
    }
}
