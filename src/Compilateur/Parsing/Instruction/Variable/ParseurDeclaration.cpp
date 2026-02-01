#include "Compilateur/Parsing/Instruction/Variable/ParseurDeclaration.h"
#include "Compilateur/AST/Noeuds/Variable/NoeudDeclaration.h"
#include "Compilateur/Lexer/TokenType.h"
#include "Compilateur/Parsing/Equation/ParseurEquation.h"
#include "Compilateur/AST/Registre/RegistreVariable.h"
#include <memory> 

ParseurDeclaration::ParseurDeclaration(std::shared_ptr<LLVMBackend> backend, std::shared_ptr<RegistreVariable> registreVariable, TokenType typeVariable)
    : _backend(std::move(backend)), _registreVariable(std::move(registreVariable)), _typeVariable(typeVariable)
{
}

ParseurDeclaration::~ParseurDeclaration()
{
}

std::shared_ptr<INoeud> ParseurDeclaration::parser(std::vector<Token>& tokens, int& index, ConstructeurArbreInstruction* constructeurArbreInstruction)
{
    consommer(tokens, index, _typeVariable, "Erreur : type attendu après 'var'");
    
    Token nomToken = consommer(tokens, index, TOKEN_IDENTIFIANT, "Erreur : nom de variable attendu");
    std::string nomVariable = nomToken.value;
    
    consommer(tokens, index, TOKEN_EGAL, "Erreur : '=' attendu après le nom de variable");
    
    ParseurEquation parseurEquation(_backend, _typeVariable, _registreVariable);
    std::shared_ptr<INoeud> expression = parseurEquation.parser(tokens, index, constructeurArbreInstruction);

    // Créer l'allocation au début du bloc d'entrée (mais après les autres allocas) : simple gestionnaire de position il déplace le curseur LLVM
    llvm::BasicBlock* insertBlock = _backend->getBuilder().GetInsertBlock();
    llvm::Instruction* positionInsertion = nullptr;
    
    if (insertBlock != nullptr) {
        // Trouver la dernière alloca dans le bloc
        for (auto& instruction : *insertBlock) {
            if (llvm::dyn_cast<llvm::AllocaInst>(&instruction) != nullptr) {
                positionInsertion = &instruction;
            }
        }
        
        if (positionInsertion != nullptr) {
            // Insérer après la dernière alloca
            llvm::Instruction* nextNode = positionInsertion->getNextNode();
            if (nextNode != nullptr) {
                _backend->getBuilder().SetInsertPoint(nextNode);
            } else {
                _backend->getBuilder().SetInsertPoint(insertBlock);
            }
        } else {
            // Aucune alloca trouvée, insérer au début
            if (insertBlock->empty()) {
                _backend->getBuilder().SetInsertPoint(insertBlock);
            } else {
                _backend->getBuilder().SetInsertPoint(&insertBlock->front());
            }
        }
    }
    
    llvm::AllocaInst* allocaInst = _backend->getBuilder().CreateAlloca(_backend->getBuilder().getFloatTy(), nullptr, nomVariable);
    
    // Restaurer le point d'insertion à la fin du bloc
    if (insertBlock != nullptr) {
        _backend->getBuilder().SetInsertPoint(insertBlock);
    }
    
    if (_registreVariable != nullptr) {
        _registreVariable->enregistrer(nomToken, allocaInst);
    }

    return std::make_shared<NoeudDeclaration>(
        _backend,
        _registreVariable,
        nomVariable,
        _backend->getBuilder().getFloatTy(), 
        expression
    );
}
