#include "Compilateur/AST/Registre/Types/TypeSimple.h"
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Type.h>
#include <stdexcept>
#include <string>

TypeSimple::TypeSimple(llvm::Type* typeLLVM)
    : _typeLLVM(typeLLVM)
{
}

auto TypeSimple::genererTypeLLVM(llvm::LLVMContext& context) -> llvm::Type*
{
    if (&_typeLLVM->getContext() == &context) {
        return _typeLLVM;
    }

    switch (_typeLLVM->getTypeID()) {
        case llvm::Type::VoidTyID:   return llvm::Type::getVoidTy(context);
        case llvm::Type::FloatTyID:  return llvm::Type::getFloatTy(context);
        case llvm::Type::DoubleTyID: return llvm::Type::getDoubleTy(context);
        case llvm::Type::IntegerTyID:
            return llvm::IntegerType::get(context, _typeLLVM->getIntegerBitWidth());
        case llvm::Type::PointerTyID:
            return llvm::PointerType::get(context, _typeLLVM->getPointerAddressSpace());
        default:
            throw std::runtime_error("Type LLVM non supporté pour la migration entre contextes : "
                                      + std::to_string(_typeLLVM->getTypeID()));
    }
}

auto TypeSimple::estFlottant() const -> bool
{
    return _typeLLVM->isFloatingPointTy();
}

auto TypeSimple::estBooleen() const -> bool
{
    return _typeLLVM->isIntegerTy(1);
}

constexpr int CHAINE_BIT_WIDTH = 8;

auto TypeSimple::estChaine() const -> bool
{
    return _typeLLVM->isIntegerTy(CHAINE_BIT_WIDTH);
}
