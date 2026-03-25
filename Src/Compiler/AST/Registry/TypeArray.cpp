#include "Compiler/AST/Registry/Types/TypeArray.h"
#include "Compiler/AST/AST_Genere.h"
#include "Compiler/AST/Nodes/Interfaces/INode.h"
#include "Compiler/AST/Registry/Types/IType.h"
#include "Compiler/Utils/PrysmaCast.h"
#include <cstdint>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Type.h>
#include <stdexcept>
#include <string>

TypeArray::TypeArray(IType* typeChild, INode* taille)
    : _typeChild(typeChild), _taille(taille)
{
}

auto TypeArray::generatedrTypeLLVM(llvm::LLVMContext& context) -> llvm::Type*
{
    llvm::Type* typeElement = _typeChild->generatedrTypeLLVM(context);

    // Si la taille est nullptr (array à taille dynamique), on returnne un pointeur
    // car en paramètre ou allocation dynamique, ça représente une adresse.
    if (_taille == nullptr) {
        return llvm::PointerType::getUnqual(context);
    }

    NodeLiteral* literal = nullptr;

    if (prysma::isa<NodeLiteral>(_taille)) {
        literal = prysma::cast<NodeLiteral>(_taille);
    }
    
    if (literal == nullptr) {
        throw std::runtime_error("Error : la taille du array doit être un littéral entier");
    }

    auto taille = static_cast<uint64_t>(std::stoi(literal->getToken().value));
    return llvm::ArrayType::get(typeElement, taille);
}

auto TypeArray::estFlottant() const -> bool
{
    return false;
}

auto TypeArray::estBooleen() const -> bool
{
    return false;
}

auto TypeArray::estChaine() const -> bool
{
    return _typeChild->estChaine();
}
