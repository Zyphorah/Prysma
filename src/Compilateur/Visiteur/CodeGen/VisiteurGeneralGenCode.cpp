#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/Noeuds/NoeudInstruction.h"
#include "Compilateur/AST/Registre/Types/TypeComplexe.h"
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>

VisiteurGeneralGenCode::VisiteurGeneralGenCode(ContextGenCode* contextGenCode, OrchestrateurInclude* orchestrateurInclude) 
: _contextGenCode(contextGenCode), _orchestrateurInclude(orchestrateurInclude)
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

std::string VisiteurGeneralGenCode::obtenirNomClasseDepuisSymbole(const Symbole& objetSymbole) {
    std::string nomClasse;
    if (objetSymbole.type != nullptr) {
        if (objetSymbole.type != nullptr && objetSymbole.type->estComplexe()) {
        auto* typeComplexe = static_cast<TypeComplexe*>(objetSymbole.type);
            nomClasse = typeComplexe->getNomClasse();
        }
    }

    if (nomClasse.empty() && objetSymbole.typePointeElement != nullptr) {
        if (auto* structType = llvm::dyn_cast<llvm::StructType>(objetSymbole.typePointeElement)) {
            llvm::StringRef structName = structType->getName();
            if (structName.starts_with("Class_")) {
                nomClasse = structName.drop_front(6).str();
            } else {
                nomClasse = structName.str();
            }
        }
    }
    return nomClasse;
}

