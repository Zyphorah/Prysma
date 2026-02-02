#include "Compilateur/AST/Noeuds/Instruction.h"

Instruction::Instruction()
{
}

void Instruction::ajouterInstruction(const std::vector<std::shared_ptr<INoeud>>& enfantsParam)
{
    for (const auto& enfant : enfantsParam) {
        enfants.push_back(enfant);
    }
}

llvm::Value* Instruction::executerEnfants()
{
    llvm::Value* resultat = nullptr;
    for (const auto& enfant : enfants) {
        resultat = enfant->genCode();
    }
    return resultat;
}

const std::vector<std::shared_ptr<INoeud>>& Instruction::getEnfants() const
{
    return enfants;
}
