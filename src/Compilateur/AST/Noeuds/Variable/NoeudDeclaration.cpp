#include "Compilateur/AST/Noeuds/Variable/NoeudDeclaration.h"
#include "Compilateur/LLVM/LLVMBackend.h"
#include "Compilateur/AST/Registre/RegistreVariable.h"

#include <llvm/IR/Instructions.h>
#include <llvm/IR/Value.h>
#include <memory>

NoeudDeclaration::NoeudDeclaration(std::shared_ptr<LLVMBackend> backend, std::shared_ptr<RegistreVariable> registreVariable, const std::string& nom, llvm::Type* type, std::shared_ptr<INoeud> expression)
    : _backend(std::move(backend)), _registreVariable(std::move(registreVariable)), _nom(nom), _type(type), _arraySize(nullptr), _expression(expression)
{
}

NoeudDeclaration::~NoeudDeclaration()
{
}

llvm::Value* NoeudDeclaration::genCode()
{
    if (_registreVariable == nullptr) {
        throw std::runtime_error("Erreur : registre de variables non initialisé");
    }
    Token nomToken;
    nomToken.value = _nom;
    nomToken.type = TOKEN_IDENTIFIANT;
    llvm::Value* val = _registreVariable->recupererVariables(nomToken);
    llvm::AllocaInst* allocaInst = llvm::dyn_cast<llvm::AllocaInst>(val);
    
    if (allocaInst == nullptr) {
        throw std::runtime_error("Erreur : variable '" + _nom + "' n'est pas une allocation valide");
    }
    
    
    // Évaluer l'expression ET initialiser la variable immédiatement
    llvm::Value* valeurCalculee = nullptr;
    if (_expression != nullptr) {
        valeurCalculee = _expression->genCode();
    }

    initialisation(allocaInst, valeurCalculee);
    
    return allocaInst;
}

llvm::AllocaInst* NoeudDeclaration::allocation()
{
    return _backend->getBuilder().CreateAlloca(_type, _arraySize, _nom);
}

llvm::AllocaInst* NoeudDeclaration::initialisation(llvm::AllocaInst* allocaInst, llvm::Value* valeur)
{
    if (valeur != nullptr && allocaInst != nullptr)
    {
        _backend->getBuilder().CreateStore(valeur, allocaInst);
    }
    return allocaInst;
}
