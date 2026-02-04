#include "Compilateur/AST/Noeuds/Fonction/NoeudDeclarationFonction.h"
#include "Compilateur/LLVM/LLVMBackend.h"
#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include "Compilateur/AST/Registre/RegistreType.h"
#include "Compilateur/Lexer/TokenType.h"
#include <llvm-18/llvm/IR/BasicBlock.h>
#include <llvm-18/llvm/IR/DerivedTypes.h>
#include <llvm-18/llvm/IR/Function.h>
#include <utility>

NoeudDeclarationFonction::NoeudDeclarationFonction(std::shared_ptr<LLVMBackend> backend, std::shared_ptr<RegistreVariable> registreVariable, std::shared_ptr<RegistreType> registreType, std::string nom, TokenType typeRetour, std::shared_ptr<ReturnContextCompilation> returnContextCompilation)
    : _backend(std::move(backend)), _registreVariable(std::move(registreVariable)), _registreType(std::move(registreType)), _nom(std::move(nom)), _typeRetourToken(typeRetour),_returnContextCompilation(std::move(returnContextCompilation))
{
}

llvm::Value* NoeudDeclarationFonction::genCode()
{
    _returnContextCompilation->piler(_typeRetourToken);
    if (_backend == nullptr) {
        throw std::runtime_error("Erreur : backend LLVM non initialisé dans NoeudDeclarationFonction");
    }

    llvm::Type* retType = _registreType->getType(_typeRetourToken);
    llvm::FunctionType* funcType = llvm::FunctionType::get(retType, false);

    llvm::Function* function = llvm::Function::Create(
        funcType,
        llvm::Function::ExternalLinkage,
        _nom,
        &_backend->getModule()
    );

    llvm::BasicBlock* entryBlock = llvm::BasicBlock::Create(_backend->getContext(), "entry", function);
    _backend->getBuilder().SetInsertPoint(entryBlock);

    // Créer une nouvelle portée pour les variables locales de la fonction
    if (_registreVariable != nullptr) {
        _registreVariable->piler();
    }

    executerEnfants();

    // Restaurer la portée précédente
    if (_registreVariable != nullptr) {
        _registreVariable->depiler();
    }

    _returnContextCompilation->depiler();

    return function;
}




