#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/Noeuds/NoeudInstruction.h"
#include <llvm/IR/Constants.h>

VisiteurGeneralGenCode::VisiteurGeneralGenCode(ContextGenCode* contextGenCode) : _contextGenCode(contextGenCode)
{}

VisiteurGeneralGenCode::~VisiteurGeneralGenCode()
{}

void VisiteurGeneralGenCode::parcourirEnfant(NoeudInstruction* noeud)
{
       for (const auto& enfant : noeud->getEnfants()) {
        enfant->accept(this);
    }
}

Symbole VisiteurGeneralGenCode::evaluerExpression(INoeud* expression) {
    if (expression != nullptr) {
        expression->accept(this);
        return _contextGenCode->valeurTemporaire;
    }
    Symbole vide;
    vide.adresse = nullptr;
    vide.type = nullptr;
    return vide;
}

