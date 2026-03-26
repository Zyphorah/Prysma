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

TypeArray::TypeArray(IType* childType, INode* size)
    : _childType(childType), _size(size)
{
}

auto TypeArray::generateLLVMType(llvm::LLVMContext& context) -> llvm::Type*
{
    llvm::Type* elementType = _childType->generateLLVMType(context);

    // If the size is nullptr (dynamic size array), return a pointer
    // because as a parameter or dynamic allocation, it represents an address.
    if (_size == nullptr) {
        return llvm::PointerType::getUnqual(context);
    }

    NodeLiteral* literal = nullptr;

    if (prysma::isa<NodeLiteral>(_size)) {
        literal = prysma::cast<NodeLiteral>(_size);
    }
    
    if (literal == nullptr) {
        throw std::runtime_error("Error: the array size must be an integer literal");
    }

    auto size = static_cast<uint64_t>(std::stoi(literal->getToken().value));
    return llvm::ArrayType::get(elementType, size);
}

auto TypeArray::isFloating() const -> bool
{
    return false;
}

auto TypeArray::isBoolean() const -> bool
{
    return false;
}

auto TypeArray::isString() const -> bool
{
    return _childType->isString();
}
