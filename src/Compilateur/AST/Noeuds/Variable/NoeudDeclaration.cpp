#include "Compilateur/AST/Noeuds/Variable/NoeudDeclaration.h"
#include "Compilateur/LLVM/LLVMBackend.h"
#include "Compilateur/AST/Registre/RegistreVariable.h"

#include <llvm-18/llvm/IR/Instructions.h>
#include <llvm-18/llvm/IR/Value.h>
#include <memory>

NoeudDeclaration::NoeudDeclaration(std::shared_ptr<LLVMBackend> backend, std::shared_ptr<RegistreVariable> registreVariable, const std::string& nom, llvm::Type* type, llvm::Value* valeur)
    : _backend(std::move(backend)), _registreVariable(std::move(registreVariable)), _nom(nom), _type(type), _arraySize(nullptr), _valeur(valeur)
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
    
    // Initialiser la variable avec sa valeur
    initialisation(allocaInst);
    
    return allocaInst;
}

llvm::AllocaInst* NoeudDeclaration::allocation()
{
    return _backend->getBuilder().CreateAlloca(_type, _arraySize, _nom);
}

llvm::AllocaInst* NoeudDeclaration::initialisation(llvm::AllocaInst* allocaInst)
{
    if (_valeur != nullptr && allocaInst != nullptr)
    {
        _backend->getBuilder().CreateStore(_valeur, allocaInst);
    }
    return allocaInst;
}
