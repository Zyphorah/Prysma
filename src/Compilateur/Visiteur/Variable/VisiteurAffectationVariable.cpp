#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"

void VisiteurGeneralGenCode::visiter(NoeudAffectationVariable* noeudAffectationVariable)
{
    /*
     // Évaluer l'expression et assigner immédiatement
    llvm::Value* valeur = nullptr;
    if (_expression != nullptr) {
        valeur = _expression->genCode();
    }
    assignation(recupererVariable(), valeur);
    
    return nullptr;
    */
}

/*
void assignation(llvm::AllocaInst* allocaInst, llvm::Value* valeur)
{
    if (valeur != nullptr && allocaInst != nullptr)
    {
        // Si la valeur est une AllocaInst (variable), charger sa valeur
        if (auto* sourceAlloca = llvm::dyn_cast<llvm::AllocaInst>(valeur)) {
            llvm::Value* loadedValue = _backend->getBuilder().CreateLoad(sourceAlloca->getAllocatedType(), sourceAlloca);
            _backend->getBuilder().CreateStore(loadedValue, allocaInst);
        } else {

            llvm::Value* valeurCast = _backend->creerAutoCast(valeur,allocaInst->getAllocatedType());
            // Sinon, assigner directement la valeur
            _backend->getBuilder().CreateStore(valeurCast, allocaInst);
        }
    }
}


llvm::AllocaInst* recupererVariable()
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

*/