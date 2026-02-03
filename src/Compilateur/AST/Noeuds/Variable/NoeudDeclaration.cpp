#include "Compilateur/AST/Noeuds/Variable/NoeudDeclaration.h"
#include "Compilateur/LLVM/LLVMBackend.h"
#include "Compilateur/AST/Registre/RegistreVariable.h"
#include "Compilateur/Lexer/TokenType.h"

#include <llvm/IR/Instructions.h>
#include <llvm/IR/Value.h>
#include <memory>
#include <utility>

NoeudDeclaration::NoeudDeclaration(std::shared_ptr<LLVMBackend> backend, std::shared_ptr<RegistreVariable> registreVariable, const std::string& nom, std::shared_ptr<INoeud> expression, std::shared_ptr<RegistreType> registreType, TokenType token)
    : _backend(std::move(backend)), _registreVariable(std::move(registreVariable)), _nom(nom), _arraySize(nullptr), _expression(std::move(expression)), _registreType(std::move(registreType)), _token(token)
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
    
     // Créer l'allocation au début du bloc d'entrée (mais après les autres allocas) : simple gestionnaire de position il déplace le curseur LLVM
    llvm::BasicBlock* insertBlock = _backend->getBuilder().GetInsertBlock();
    llvm::Instruction* positionInsertion = nullptr;
    
    if (insertBlock != nullptr) {
        for (auto& instruction : *insertBlock) {
            if (llvm::dyn_cast<llvm::AllocaInst>(&instruction) != nullptr) {
                positionInsertion = &instruction;
            }
        }
        
        if (positionInsertion != nullptr) {
            llvm::Instruction* nextNode = positionInsertion->getNextNode();
            if (nextNode != nullptr) {
                _backend->getBuilder().SetInsertPoint(nextNode);
            } else {
                _backend->getBuilder().SetInsertPoint(insertBlock);
            }
        } else {
            if (insertBlock->empty()) {
                _backend->getBuilder().SetInsertPoint(insertBlock);
            } else {
                _backend->getBuilder().SetInsertPoint(&insertBlock->front());
            }
        }
    }
    
    llvm::AllocaInst* allocaInst = allocation();
    
    if (insertBlock != nullptr) {
        _backend->getBuilder().SetInsertPoint(insertBlock);
    }
    
    _registreVariable->enregistrer(nomToken, allocaInst);
    
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
    return _backend->getBuilder().CreateAlloca(_registreType->recuperer(_token), _arraySize, _nom);
}

llvm::AllocaInst* NoeudDeclaration::initialisation(llvm::AllocaInst* allocaInst, llvm::Value* valeur)
{
    if (valeur != nullptr && allocaInst != nullptr)
    {
        _backend->getBuilder().CreateStore(valeur, allocaInst);
    }
    return allocaInst;
}
