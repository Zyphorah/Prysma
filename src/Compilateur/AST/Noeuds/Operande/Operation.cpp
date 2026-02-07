#include "Compilateur/AST/Noeuds/Operande/Operation.h"
#include <llvm/IR/Value.h>
#include <stdexcept>
#include <functional>

Operation::Operation(const std::function<llvm::Value*(llvm::Value*, llvm::Value*)>& operateur)
    : _droite(nullptr), _gauche(nullptr), _operateur(operateur) {
}

std::shared_ptr<IExpression> Operation::ajouterExpression(
    std::shared_ptr<INoeud> gauche, 
    std::shared_ptr<INoeud> droite) {
    
    if (!droite || !gauche) {
        throw std::invalid_argument("Les noeuds fournis sont nuls");
    }
    
    if (_droite == nullptr && _gauche == nullptr) {
        _gauche = gauche;
        _droite = droite;
    } else {
        throw std::logic_error("Les noeuds existent déjà pour cette opération");
    }
    return nullptr;
}

void Operation::accept(IVisiteur* visiteur)
{
    visiteur->visiter(this);
}

/*

llvm::Value* Operation::genCode() {
    if (!_gauche || !_droite) {
        throw std::runtime_error("Erreur: opérande gauche ou droite manquant dans Operation::genCode()");
    }
    
    llvm::Value* valGauche = _gauche->genCode();
    llvm::Value* valDroite = _droite->genCode();
    
    if (valGauche == nullptr || valDroite == nullptr) {
        throw std::runtime_error("Erreur: l'évaluation d'une opérande a retourné nullptr");
    }
    return _operateur(valGauche, valDroite);
}
*/