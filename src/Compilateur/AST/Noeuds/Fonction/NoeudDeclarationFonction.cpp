#include "Compilateur/AST/Noeuds/Fonction/NoeudDeclarationFonction.h"
#include "Compilateur/LLVM/LLVMBackend.h"
#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include "Compilateur/Lexer/TokenType.h"
#include <llvm-18/llvm/IR/BasicBlock.h>
#include <llvm-18/llvm/IR/DerivedTypes.h>
#include <llvm-18/llvm/IR/Function.h>
#include <utility>

NoeudDeclarationFonction::NoeudDeclarationFonction(std::shared_ptr<LLVMBackend> backend, std::shared_ptr<RegistreVariable> registreVariable, std::string nom, TokenType typeRetour)
    : _backend(std::move(backend)), _registreVariable(std::move(registreVariable)), _nom(std::move(nom)), _typeRetourToken(typeRetour)
{
}

llvm::Value* NoeudDeclarationFonction::genCode()
{
    if (_backend == nullptr) {
        throw std::runtime_error("Erreur : backend LLVM non initialisé dans NoeudDeclarationFonction");
    }

    llvm::Type* retType;
    if (_typeRetourToken == TOKEN_TYPE_INT) {
        retType = llvm::Type::getInt32Ty(_backend->getContext());
    } else if (_typeRetourToken == TOKEN_TYPE_FLOAT) {
        retType = llvm::Type::getFloatTy(_backend->getContext());
    } else if (_typeRetourToken == TOKEN_TYPE_VOID) {
        retType = llvm::Type::getVoidTy(_backend->getContext());
    } else {
        throw std::runtime_error("Erreur : Type de token inconnu: " + std::to_string(_typeRetourToken));
    }

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

    llvm::Value* resultatCorps = executerEnfants();

    // Restaurer la portée précédente
    if (_registreVariable != nullptr) {
        _registreVariable->depiler();
    }

    if (_backend->getBuilder().GetInsertBlock()->getTerminator() == nullptr) {
        if (retType->isVoidTy()) {
            _backend->getBuilder().CreateRetVoid();
        } else if (resultatCorps != nullptr) {
            _backend->getBuilder().CreateRet(resultatCorps);
        } else {
            _backend->getBuilder().CreateRet(llvm::ConstantInt::get(retType, 0));
        }
    }

    return function;
}




