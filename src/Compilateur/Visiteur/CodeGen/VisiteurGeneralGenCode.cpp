#include "Compilateur/Visiteur/CodeGen/VisiteurGeneralGenCode.h"
#include "Compilateur/AST/Noeuds/Interfaces/INoeud.h"
#include "Compilateur/AST/Noeuds/NoeudInstruction.h"
#include "Compilateur/AST/Registre/ContextGenCode.h"
#include "Compilateur/AST/Registre/Pile/RegistreVariable.h"
#include "Compilateur/AST/Registre/Types/TypeComplexe.h"
#include "Compilateur/Utils/PrysmaCast.h"
#include <cstddef>
#include <llvm/ADT/StringRef.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/Support/Casting.h>
#include <string>

VisiteurGeneralGenCode::VisiteurGeneralGenCode(ContextGenCode* contextGenCode, OrchestrateurInclude* orchestrateurInclude) 
: _contextGenCode(contextGenCode), _orchestrateurInclude(orchestrateurInclude)
{}

VisiteurGeneralGenCode::~VisiteurGeneralGenCode()
= default;

void VisiteurGeneralGenCode::parcourirEnfant(NoeudInstruction* noeud)
{
       for (const auto& enfant : noeud->getEnfants()) {
        enfant->accept(this);
    }
}

auto VisiteurGeneralGenCode::evaluerExpression(INoeud* expression) -> Symbole {
    if (expression != nullptr) {
        expression->accept(this);
        return _contextGenCode->getValeurTemporaire();
    }
    Symbole vide;
    vide = Symbole(nullptr, nullptr, nullptr);
    
    return vide;
}

auto VisiteurGeneralGenCode::obtenirNomClasseDepuisSymbole(const Symbole& objetSymbole) -> std::string {
    std::string nomClasse;
    if (objetSymbole.getType() != nullptr) {
        if (auto* typeComplexe = prysma::dyn_cast<TypeComplexe>(objetSymbole.getType())) {
            nomClasse = typeComplexe->getNomClasse();
        }
    }

    constexpr size_t CLASS_PREFIX_LENGTH = 6;
    if (nomClasse.empty() && objetSymbole.getTypePointeElement() != nullptr) {
        if (auto* structType = llvm::dyn_cast<llvm::StructType>(objetSymbole.getTypePointeElement())) {
            llvm::StringRef structName = structType->getName();
            if (structName.starts_with("Class_")) {
                nomClasse = structName.drop_front(CLASS_PREFIX_LENGTH).str();
            } else {
                nomClasse = structName.str();
            }
        }
    }
    return nomClasse;
}

