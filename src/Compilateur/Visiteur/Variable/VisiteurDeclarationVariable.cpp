#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"

void VisiteurGeneralGenCode::visiter(NoeudDeclarationVariable* noeudDeclarationVariable) 
{
    /*
     if (_registreVariable == nullptr) {
        throw std::runtime_error("Erreur : registre de variables non initialisé");
    }

    Token nomToken;
    nomToken.value = _nom;
    nomToken.type = TOKEN_IDENTIFIANT;
    
     // Créer l'allocation au début du bloc d'entrée (mais après les autres allocas) : simple gestionnaire de position il déplace le curseur LLVM
    llvm::BasicBlock* insertBlock = _backend->getBuilder().GetInsertBlock();
    llvm::NoeudInstruction* positionInsertion = nullptr;
    
    if (insertBlock != nullptr) {
        for (auto& instruction : *insertBlock) {
            if (llvm::dyn_cast<llvm::AllocaInst>(&instruction) != nullptr) {
                positionInsertion = &instruction;
            }
        }
        
        if (positionInsertion != nullptr) {
            llvm::NoeudInstruction* nextNode = positionInsertion->getNextNode();
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
    */
}

/*
llvm::AllocaInst* allocation()
{
    return _backend->getBuilder().CreateAlloca(_registreType->recuperer(_token), _arraySize, _nom);
}

llvm::AllocaInst* initialisation(llvm::AllocaInst* allocaInst, llvm::Value* valeur)
{
    if (valeur != nullptr && allocaInst != nullptr)
    {
        _backend->getBuilder().CreateStore(_backend->creerAutoCast(valeur,_registreType->recuperer(_token)), allocaInst);
    }
    return allocaInst;
}
*/