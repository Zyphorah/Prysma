#include "compiler/ast/registry/types/type_array.h"
#include "compiler/ast/ast_genere.h"
#include "compiler/ast/nodes/interfaces/i_node.h"
#include "compiler/ast/registry/types/i_type.h"
#include "compiler/utils/prysma_cast.h"
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

    auto size = static_cast<uint64_t>(std::stoull(literal->getToken().value));
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
