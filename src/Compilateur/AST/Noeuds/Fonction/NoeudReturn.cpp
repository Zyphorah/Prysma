#include "Compilateur/AST/Noeuds/Fonction/NoeudReturn.h"
#include "Compilateur/LLVM/LLVMBackend.h"
#include <llvm-18/llvm/IR/Instructions.h>
#include <utility>

NoeudReturn::NoeudReturn(std::shared_ptr<LLVMBackend> backend, std::shared_ptr<INoeud> valeurRetour, std::shared_ptr<ReturnContextCompilation> returnContextCompilation, std::shared_ptr<RegistreType> registreType)
    : _backend(std::move(backend)), _valeurRetour(std::move(valeurRetour)), _returnContextCompilation(std::move(returnContextCompilation)), _registreType(std::move(registreType))
{
}

llvm::Value* NoeudReturn::genCode()
{
    llvm::Value* valeur = _valeurRetour->genCode();
    if (valeur == nullptr) {
        throw std::runtime_error("Erreur : impossible d'évaluer l'expression de retour");
    }

    // Récupérer le type de retour depuis le contexte
    TokenType typeRetourToken = _returnContextCompilation->recupererContext();
    llvm::Type* typeRetour = _registreType->getType(typeRetourToken);
    
   
    valeur = _backend->getBuilder().CreateFPToSI(valeur, typeRetour);
    
    
    _backend->getBuilder().CreateRet(valeur);
    return valeur;
}
