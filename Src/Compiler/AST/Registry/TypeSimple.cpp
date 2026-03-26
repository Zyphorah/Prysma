#include "Compiler/AST/Registry/Types/TypeSimple.h"
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Type.h>
#include <stdexcept>
#include <string>

TypeSimple::TypeSimple(llvm::Type* llvmType)
    : _llvmType(llvmType)
{
}

auto TypeSimple::generateLLVMType(llvm::LLVMContext& context) -> llvm::Type*
{
    if (&_llvmType->getContext() == &context) {
        return _llvmType;
    }

    switch (_llvmType->getTypeID()) {
        case llvm::Type::VoidTyID:   return llvm::Type::getVoidTy(context);
        case llvm::Type::FloatTyID:  return llvm::Type::getFloatTy(context);
        case llvm::Type::DoubleTyID: return llvm::Type::getDoubleTy(context);
        case llvm::Type::IntegerTyID:
            return llvm::IntegerType::get(context, _llvmType->getIntegerBitWidth());
        case llvm::Type::PointerTyID:
            return llvm::PointerType::get(context, _llvmType->getPointerAddressSpace());
        default:
            throw std::runtime_error("Unsupported LLVM type for migration between contexts: "
                                      + std::to_string(_llvmType->getTypeID()));
    }
}

auto TypeSimple::isFloating() const -> bool
{
    return _llvmType->isFloatingPointTy();
}

auto TypeSimple::isBoolean() const -> bool
{
    return _llvmType->isIntegerTy(1);
}

constexpr int STRING_BIT_WIDTH = 8;

auto TypeSimple::isString() const -> bool
{
    return _llvmType->isIntegerTy(STRING_BIT_WIDTH);
}
