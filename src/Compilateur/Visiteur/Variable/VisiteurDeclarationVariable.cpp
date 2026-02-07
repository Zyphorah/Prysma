#include "Compilateur/AST/Registre/ContextGenCode.h"
#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/Noeuds/Variable/NoeudDeclarationVariable.h"


llvm::AllocaInst* allocation(ContextGenCode* context, TokenType tokenType, const std::string& nom);
llvm::AllocaInst* initialisation(ContextGenCode* context, llvm::AllocaInst* allocaInst, llvm::Value* valeur, TokenType tokenType);

void VisiteurGeneralGenCode::visiter(NoeudDeclarationVariable* noeudDeclarationVariable) 
{
    
    if (_contextGenCode == nullptr || _contextGenCode->registreVariable == nullptr) {
        throw std::runtime_error("Erreur : registre de variables non initialisé");
    }

    Token nomToken;
    nomToken.value = noeudDeclarationVariable->_nom;
    nomToken.type = TOKEN_IDENTIFIANT;
    
    // Créer l'allocation au début du bloc d'entrée (mais après les autres allocas) : simple gestionnaire de position il déplace le curseur LLVM
    llvm::BasicBlock* insertBlock = _contextGenCode->backend->getBuilder().GetInsertBlock();
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
                _contextGenCode->backend->getBuilder().SetInsertPoint(nextNode);
            } else {
                _contextGenCode->backend->getBuilder().SetInsertPoint(insertBlock);
            }
        } else {
            if (insertBlock->empty()) {
                _contextGenCode->backend->getBuilder().SetInsertPoint(insertBlock);
            } else {
                _contextGenCode->backend->getBuilder().SetInsertPoint(&insertBlock->front());
            }
        }
    }
    
    llvm::AllocaInst* allocaInst = allocation(_contextGenCode.get(), noeudDeclarationVariable->_token, noeudDeclarationVariable->_nom);
    
    if (insertBlock != nullptr) {
        _contextGenCode->backend->getBuilder().SetInsertPoint(insertBlock);
    }
    
    _contextGenCode->registreVariable->enregistrer(nomToken, allocaInst);
    
    // Évaluer l'expression ET initialiser la variable immédiatement
    llvm::Value* valeurCalculee = nullptr;
    if (noeudDeclarationVariable->_expression != nullptr) {
        noeudDeclarationVariable->_expression->accept(this);
        valeurCalculee = _contextGenCode->valeurTemporaire;
    }

    initialisation(_contextGenCode.get(), allocaInst, valeurCalculee, noeudDeclarationVariable->_token);
}



llvm::AllocaInst* allocation(ContextGenCode* context, TokenType tokenType, const std::string& nom)
{
    return context->backend->getBuilder().CreateAlloca(context->registreType->recuperer(tokenType), nullptr, nom);
}

llvm::AllocaInst* initialisation(ContextGenCode* context, llvm::AllocaInst* allocaInst, llvm::Value* valeur, TokenType tokenType)
{
    if (valeur != nullptr && allocaInst != nullptr)
    {
        context->backend->getBuilder().CreateStore(context->backend->creerAutoCast(valeur,context->registreType->recuperer(tokenType)), allocaInst);
    }
    return allocaInst;
}
