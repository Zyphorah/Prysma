#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"

VisiteurGeneralGenCode::VisiteurGeneralGenCode(std::shared_ptr<ContextGenCode> contextGenCode) : _contextGenCode(std::move(contextGenCode))
{}

VisiteurGeneralGenCode::~VisiteurGeneralGenCode()
{}


void VisiteurGeneralGenCode::parcourirEnfant(NoeudInstruction* noeud)
{
       for (const auto& enfant : noeud->getEnfants()) {
        enfant->accept(this);
    }
}