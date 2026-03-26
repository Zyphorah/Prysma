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

void GeneralVisitorGenCode::traverseChild(NodeInstruction* node)
{
    for (const auto& child : node->getChildren()) {
        child->accept(this);
    }
}

auto GeneralVisitorGenCode::evaluateExpression(INode* expression) -> Symbol {
    if (expression != nullptr) {
        expression->accept(this);
        return _contextGenCode->getTemporaryValue();
    }
    Symbol empty;
    empty = Symbol(nullptr, nullptr, nullptr);
    return empty;
}

auto GeneralVisitorGenCode::getClassNameFromSymbol(const Symbol& objectSymbol) -> std::string {
    std::string className;
    if (objectSymbol.getType() != nullptr) {
        if (auto* typeComplex = prysma::dyn_cast<TypeComplex>(objectSymbol.getType())) {
            className = typeComplex->getClassName();
        }
    }

    constexpr size_t CLASS_PREFIX_LENGTH = 6;
    if (className.empty() && objectSymbol.getPointedElementType() != nullptr) {
        if (auto* structType = llvm::dyn_cast<llvm::StructType>(objectSymbol.getPointedElementType())) {
            llvm::StringRef structName = structType->getName();
            if (structName.starts_with("Class_")) {
                className = structName.drop_front(CLASS_PREFIX_LENGTH).str();
            } else {
                className = structName.str();
            }
        }
    }
    return className;
}

