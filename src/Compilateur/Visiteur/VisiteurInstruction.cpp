#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/Noeuds/NoeudInstruction.h"

void VisiteurGeneralGenCode::visiter(NoeudInstruction* instruction)
{
    if (instruction != nullptr) {
        // Parcourir tous les enfants de l'instruction
        for (const auto& enfant : instruction->getEnfants()) {
            if (enfant != nullptr) {
                enfant->accept(this);
            }
        }
    }
}
