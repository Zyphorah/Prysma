#include "Compiler/Visitor/CodeGen/VisitorGeneralGenCode.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Nodes/NodeInstruction.h"
#include "Compiler/AST/Registry/ContextGenCode.h"
#include "Compiler/AST/Registry/Stack/RegistryVariable.h"
#include "Compiler/AST/Registry/Types/TypeComplex.h"
#include "Compiler/Utils/PrysmaCast.h"
#include <cstddef>
#include <llvm/ADT/StringRef.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/Support/Casting.h>
#include <string>

GeneralVisitorGenCode::GeneralVisitorGenCode(ContextGenCode* contextGenCode, OrchestratorInclude* orchestratorInclude) 
: _contextGenCode(contextGenCode), _orchestratorInclude(orchestratorInclude)
{}

GeneralVisitorGenCode::~GeneralVisitorGenCode()
= default;

void GeneralVisitorGenCode::parcourirChild(NodeInstruction* node)
{
       for (const auto& child : node->getChilds()) {
        child->accept(this);
    }
}

auto GeneralVisitorGenCode::evaluerExpression(INode* expression) -> Symbole {
    if (expression != nullptr) {
        expression->accept(this);
        return _contextGenCode->getValeurTemporaire();
    }
    Symbole vide;
    vide = Symbole(nullptr, nullptr, nullptr);
    
    return vide;
}

auto GeneralVisitorGenCode::obtenirNomClasseDepuisSymbole(const Symbole& objectSymbole) -> std::string {
    std::string nomClasse;
    if (objectSymbole.getType() != nullptr) {
        if (auto* typeComplex = prysma::dyn_cast<TypeComplex>(objectSymbole.getType())) {
            nomClasse = typeComplex->getNomClasse();
        }
    }

    constexpr size_t CLASS_PREFIX_LENGTH = 6;
    if (nomClasse.empty() && objectSymbole.getTypePointeElement() != nullptr) {
        if (auto* structType = llvm::dyn_cast<llvm::StructType>(objectSymbole.getTypePointeElement())) {
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

