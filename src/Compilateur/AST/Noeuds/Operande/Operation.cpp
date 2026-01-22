#include "AST/Noeuds/Operande/Operation.h"
#include <llvm-18/llvm/IR/Value.h>
#include <stdexcept>
#include <functional>

Operation::Operation(const std::function<llvm::Value*(llvm::Value*, llvm::Value*)>& operateur)
    : _droite(nullptr), _gauche(nullptr), _operateur(operateur) {}

std::shared_ptr<INoeud> Operation::ajouterExpression(
    std::shared_ptr<IExpression> gauche, 
    std::shared_ptr<IExpression> droite) {
    
    if (!droite || !gauche) {
        throw std::invalid_argument("Les noeuds fournis sont nuls");
    }
    
    if (_droite == nullptr && _gauche == nullptr) {
        _gauche = gauche;
        _droite = droite;
    } else {
        throw std::logic_error("Les noeuds existent déjà pour cette opération");
    }
    
    return std::make_shared<Operation>(*this);
}

llvm::Value* Operation::resoudre() {
    return _operateur(_gauche->resoudre(), _droite->resoudre());
}
